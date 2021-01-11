#include "TextureArray.h"
#include <base/opengl/util/GLUtil.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual/Texture.h>

using namespace graphics;

int TextureArray::Stats::vramBytes = 0;

const int TextureArray::MAX_TEXTURES_COUNT = 32;
int TextureArray::GLOBAL_TEXTURE_ARRAYS_COUNT = 0;

TextureArray::TextureArray(
	int uIndex,
	int width, int height, int bitsPerPixel_NOT_BYTES,
	TextureFilterMin const* minFilter, TextureFilterMag const* magFilter,
	TextureWrap const* wrapS, TextureWrap const* wrapT,
	int minTexturesCapacity, int hardcodedTextureBindSlot)
	: super(&BindLocations::TEXTURE),
	uIndex(uIndex),
	width(width), height(height), bitsPerPixel_NOT_BYTES(bitsPerPixel_NOT_BYTES),
	minFilter(minFilter), magFilter(magFilter),
	wrapS(wrapS), wrapT(wrapT),
	minTexturesCapacity(minTexturesCapacity), hardcodedTextureBindSlot(hardcodedTextureBindSlot), texturesList(minTexturesCapacity)
{
	resourceHasOptionalGHandle = true;
}

int TextureArray::getWidth() {
	return width;
}

int TextureArray::getHeight() {
	return height;
}

int TextureArray::getBitsPerPixel_NOT_BYTES() {
	return bitsPerPixel_NOT_BYTES;
}

int TextureArray::getBytesPerPixel_NOT_BITS() {
	if ((bitsPerPixel_NOT_BYTES % 8) != 0) {
		// The texture bitsPerPixel_NOT_BYTES is not a multiple of 8. Conversion is not valid.
		/// This function should be used only on multiple of 8 bitsPerPixel_NOT_BYTES.
		throw LogicException(LOC);
	}

	return bitsPerPixel_NOT_BYTES / 8;
}

bool TextureArray::hookTextureIfCanManage(Texture* texture) {
	if (texture->textureArray != nullptr) {
		// Texture already added.
		throw LogicException(LOC);
	}

	if (width != texture->getWidth() || height != texture->getHeight() || getBytesPerPixel_NOT_BITS() != texture->getBytesPerPixel_NOT_BITS()) {
		// The texture params are different from the texture array.
		return false;
	}
	if (!minFilter->equals(texture->minFilter) || !magFilter->equals(texture->magFilter)) {
		// The texture params are different from the texture array.
		return false;
	}
	if (!wrapS->equals(texture->wrapS) || !wrapT->equals(texture->wrapT)) {
		// The texture params are different from the texture array.
		return false;
	}

	hookTexture(texture);

	return true;
}

void TextureArray::hookTexture(Texture* texture) {
	if (texture->textureArray != nullptr) {
		// Texture already added.
		throw LogicException(LOC);
	}
	if (width != texture->getWidth() || height != texture->getHeight() || getBytesPerPixel_NOT_BITS() != texture->getBytesPerPixel_NOT_BITS()) {
		// The texture params are different from the texture array.
		throw LogicException(LOC);
	}
	if (!minFilter->equals(texture->minFilter) || !magFilter->equals(texture->magFilter)) {
		// The texture params are different from the texture array.
		throw LogicException(LOC);
	}
	if(!wrapS->equals(texture->wrapS) || !wrapT->equals(texture->wrapT)) {
		// The texture params are different from the texture array.
		throw LogicException(LOC);
	}

	// Find first emty slot.
	for (int i = 0; i < texturesList.count(); i++) {
		Texture* tex = texturesList.getDirect(i);

		if (tex == nullptr) {
			// Hook the texture.
			texturesList.setDirect(i, texture);

			texture->inTextureArrayIndex = i;
			texture->packedTextureIndex = uIndex* TextureArray::MAX_TEXTURES_COUNT + texture->inTextureArrayIndex;
			texture->textureArray = this;

			texture->invalidateData();

			return;
		}
	}

	if (texturesList.count() >= TextureArray::MAX_TEXTURES_COUNT) {
		// There should never be so many textures, a error probably occured.
		throw LogicException(LOC);
	}

	// Increate the textures slot and do a full re-upload with re-allocation of vram.

	texturesList.resizeWithElementsMove(texturesList.count() + 1);

	texturesList.setDirect(texturesList.count() - 1, texture);

	texture->inTextureArrayIndex = texturesList.count() - 1;
	texture->packedTextureIndex = uIndex * TextureArray::MAX_TEXTURES_COUNT + texture->inTextureArrayIndex;
	texture->textureArray = this;

	texture->invalidateData();

	invalidAllocationBlock();
}

void TextureArray::releaseTexture(Texture* texture) {
	if (texture->inTextureArrayIndex >= texturesList.count() || texturesList.getDirect(texture->inTextureArrayIndex) != texture) {
		// Texture is not in the list where it should be.
		throw LogicException(LOC);
	}

	int removedTextureIndex = texture->inTextureArrayIndex;

	texturesList.setDirect(texture->inTextureArrayIndex, nullptr);
	texture->inTextureArrayIndex = -1;
	texture->packedTextureIndex = -1;
	texture->invalidateData();

	if (texturesList.count() > minTexturesCapacity && removedTextureIndex == texturesList.count() - 1) {
		// Just removed the last texture, try to remove textures if needed.
		// Start from the end and go down to find the new capacity.
		int newCapacity = texturesList.count();
		while (newCapacity > 0 && newCapacity > minTexturesCapacity && texturesList.getDirect(newCapacity - 1) == nullptr) {
			newCapacity--;
		}
		if (newCapacity < texturesList.count()) {
			// Do the array shrinking.
			texturesList.resizeWithElementsMove(newCapacity);

			invalidAllocationBlock();
		}
	}
}

void TextureArray::onLoadToGl() {
	super::onLoadToGl();

	if (!GLUtil::gl->getIsGlContext()) {
		// No gl context or texture already loaded using the current gl context.
		throw LogicException(LOC);
	}

	if (minFilter->usesMipMapping || magFilter->usesMipMapping) {
		// Current logic doesn't support mip mapping;
		assert(false);
	}

	if (texturesList.count() == 0) {
		// There are no textures to upload;
		return;
	}

	CallGLChecked(GLUtil::gl->genTextures(1, &gHandle));

	// Use a slot of 0 because this type of asset is indexed.
	bind(hardcodedTextureBindSlot/*slot*/, false/*isStorageAllocated*/);

	CallGLChecked(loggedTextureSizeBytes = GLUtil::gl->texStorage3D(
		IGL::TEXTURE_2D_ARRAY/*target*/,
		1/*levels*/,
		IGL::RGBA8/*internalformat*/,
		width/*width*/,
		height/*height*/,
		texturesList.count()/*depth*/
	));
	Stats::vramBytes += (4/*channels*/ * 1/*bytes_per_channel*/)/*RGBA8*/ * width * height * texturesList.count();

	CallGLChecked(GLUtil::gl->texParameteri(IGL::TEXTURE_2D_ARRAY, IGL::TEXTURE_MIN_FILTER, minFilter->glId));
	CallGLChecked(GLUtil::gl->texParameteri(IGL::TEXTURE_2D_ARRAY, IGL::TEXTURE_MAG_FILTER, magFilter->glId));

	CallGLChecked(GLUtil::gl->texParameteri(IGL::TEXTURE_2D_ARRAY, IGL::TEXTURE_WRAP_S, wrapS->glId));
	CallGLChecked(GLUtil::gl->texParameteri(IGL::TEXTURE_2D_ARRAY, IGL::TEXTURE_WRAP_T, wrapT->glId));

	//unbind();

	isInvalidData = true;
}

void TextureArray::onUnloadFromGl() {
	if (gHandle != 0) {
		Stats::vramBytes -= (4/*channels*/ * 1/*bytes_per_channel*/)/*RGBA8*/ * width * height * texturesList.count();

		CallGLChecked(GLUtil::gl->deleteTextures(1, &gHandle, loggedTextureSizeBytes));

		gHandle = 0;
	}

	isInvalidData = true;

	super::onUnloadFromGl();
}

void TextureArray::onValidateData() {
	super::onValidateData();

	// This force updates all textures.
	// This occurs at start and when the texture array size changes.
	// This function requires to force validate all textures.
	isInvalidData = true;
}

void TextureArray::updateGLBufferIfNeeded(graphics::ShaderProgram* shader) {
	super::updateGLBufferIfNeeded(shader);

	if (isInvalidData) {
		isInvalidData = false;

		// Upload all textures.
		for (int i = 0; i < texturesList.count(); i++) {
			Texture* texture = texturesList.getDirect(i);

			if (texture != nullptr) {
				texture->invalidateData();
			}
		}
	}

	for (int i = 0; i < texturesList.count(); i++) {
		Texture* texture = texturesList.getDirect(i);

		if (texture != nullptr) {
			texture->updateGLBufferIfNeeded(shader);
		}
	}
}

void TextureArray::onBind(int slot, graphics::ShaderProgram* shader) {
	super::onBind(slot, shader);

	if (!GLUtil::gl->getIsGlContext() || (hardcodedTextureBindSlot >= 0 && slot != hardcodedTextureBindSlot)) {
		throw LogicException(LOC);
	}

	if (gHandle != 0) {
		CallGLChecked(GLUtil::gl->activeTexture(IGL::TEXTURE0 + slot));
		CallGLChecked(GLUtil::gl->bindTexture(IGL::TEXTURE_2D_ARRAY, gHandle));
	} else {
		if (texturesList.count() > 0) {
			// There are textures, gHandle should exist and this case should not occur;
			throw LogicException(LOC);
		}
	}
}

void TextureArray::onUnbind(int slot, graphics::ShaderProgram* shader) {
	super::onUnbind(slot, shader);

	if (!GLUtil::gl->getIsGlContext()) {
		throw LogicException(LOC);
	}

	if (gHandle != 0) {
		CallGLChecked(GLUtil::gl->activeTexture(IGL::TEXTURE0 + slot));
		CallGLChecked(GLUtil::gl->bindTexture(IGL::TEXTURE_2D_ARRAY, 0));
	} else {
		if (texturesList.count() > 0) {
			// There are textures, gHandle should exist and this case should not occur;
			// Note. This is not an error, it occurs when the first texture is added need to unload,load to gl because the size changes.
			//throw LogicException(LOC);
		}
	}
}

int TextureArray::getBitsPerPixel_NOT_BYTES() const {
	return bitsPerPixel_NOT_BYTES;
}

int TextureArray::getBytesPerPixel_NOT_BITS() const {
	if ((bitsPerPixel_NOT_BYTES % 8) != 0) {
		// The texture bitsPerPixel_NOT_BYTES is not a multiple of 8. Conversion is not valid.
		/// This function should be used only on multiple of 8 bitsPerPixel_NOT_BYTES.
		throw LogicException(LOC);
	}

	return bitsPerPixel_NOT_BYTES / 8;
}

TextureArray::~TextureArray() {
	//void
}
