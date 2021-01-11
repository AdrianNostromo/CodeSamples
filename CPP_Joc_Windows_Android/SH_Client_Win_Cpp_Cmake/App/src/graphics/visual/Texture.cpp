#include "Texture.h"
#include <base/opengl/util/GLUtil.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual/TextureArray.h>
#include <vendor/stb/stb_image.h>
#include <vendor/stb/stb_image_write.h>

int Texture::Stats::texturesCount = 0;
int Texture::Stats::ramBytes = 0;
int Texture::Stats::vramBytes = 0;

Texture::Texture(
	base::Buffer buffer, bool isDataPacked,
    int width, int height, int channelsCount, int bitsPerPixel_NOT_BYTES,
	unsigned int mipMapLevels, GLenum sizedInternalFormat,
	Texture::TextureFilterMin const* minFilter, Texture::TextureFilterMag const* magFilter,
    Texture::TextureWrap const* wrapS, Texture::TextureWrap const* wrapT,
	ArrayList<graphics::TextureArray*>* globalTextures,
	Color* borderColorManaged, int autoBindSlot
)
	: super(&BindLocations::TEXTURE),
	buffer(buffer), isDataPacked(isDataPacked),
	width(width), height(height), channelsCount(channelsCount), bitsPerPixel_NOT_BYTES(bitsPerPixel_NOT_BYTES),
	mipMapLevels(mipMapLevels), sizedInternalFormat(sizedInternalFormat),
	minFilter(minFilter), magFilter(magFilter),
	wrapS(wrapS), wrapT(wrapT),
	borderColorManaged(borderColorManaged), autoBindSlot(autoBindSlot)
{
	if(borderColorManaged != nullptr && IGL::TEXTURE_BORDER_COLOR == 0) {
		// Feature not supported. On android, this must be done in the shader. Currently don't specify this parameter. Maybe in the future just ignore it but not now.
		throw LogicException(LOC);
	}
	if((wrapS->glId == IGL::CLAMP_TO_BORDER || wrapT->glId == IGL::CLAMP_TO_BORDER) && IGL::CLAMP_TO_BORDER == 0) {
		// Feature not supported. On android, this must be done in the shader. Currently don't specify this parameter. Maybe in the future just ignore it but not now.
		throw LogicException(LOC);
	}

	if (globalTextures != nullptr) {
		for (int i = 0; i < globalTextures->count(); i++) {
			graphics::TextureArray* ta = globalTextures->getDirect(i);
			if (ta->hookTextureIfCanManage(this)) {
				break;
			}
		}
		if (textureArray == nullptr) {
			// No TextureArray found that can manage this texture. It is required to find one.
			throw LogicException(LOC);
		}
	}

	Stats::texturesCount++;
	if (buffer != nullptr) {
		Stats::ramBytes += buffer.getBytesCount();
	}
}

void Texture::createMain() {
    super::createMain();

    //void
}

void Texture::onLoadToGl() {
    super::onLoadToGl();
	
	if (textureArray != nullptr) {
		// Use array loading logic.
		// Use a slot of 0 because this type of asset is indexed.
		if (textureArray->getHardcodedTextureBindSlot() >= 0) {
			textureArray->bind(textureArray->getHardcodedTextureBindSlot()/*slot*/);
		} else {
			textureArray->bind(TextureBindSlots::temp);
		}

		// Data upload is done in the onValidateData because it is dynamic sometimes.
		//CallGLChecked(GLUtil::gl->texSubImage3D(
		//	IGL::TEXTURE_2D_ARRAY/*target*/, 0/*level*/,
		//	0/*xoffset*/, 0/*yoffset*/, textureIndex/*zoffset*/,
		//	width/*width*/, height/*height*/, 1/*depth*/,
		//	IGL::RGBA/*format*/, IGL::UNSIGNED_BYTE/*type*/, unpackedBuffer.getBlock()/*data*/
		//));
		//
		//dataValidatedOnLoadToGl();
	} else {
		if (!GLUtil::gl->getIsGlContext()) {
			// No gl context or texture already loaded using the current gl context.
			throw LogicException(LOC);
		}

		if (minFilter->usesMipMapping || magFilter->usesMipMapping) {
			// Current logic doesn't support mip mapping;
			assert(false);
		}

		CallGLChecked(GLUtil::gl->genTextures(1, &gHandle));

		bind(autoBindSlot, false/*isStorageAllocated*/);

		// This is used to allocate in a single call the entire needed memory for the texture and mip maps levels.
		CallGLChecked(loggedTextureSizeBytes = GLUtil::gl->texStorage2D(
			IGL::TEXTURE_2D/*target*/,
			mipMapLevels/*levels*/,
			sizedInternalFormat/*internalformat*/,
			width/*width*/,
			height/*height*/
		));
		if (sizedInternalFormat == IGL::DEPTH_COMPONENT32F) {
			Stats::vramBytes += (1/*channels*/ * 4/*bytes_per_channel*/)/*RGBA8*/ * width * height;
		} else {
			// Unchecked format, add a case for it.
			throw LogicException(LOC);
		}

		CallGLChecked(GLUtil::gl->texParameteri(IGL::TEXTURE_2D, IGL::TEXTURE_MIN_FILTER, minFilter->glId));
		CallGLChecked(GLUtil::gl->texParameteri(IGL::TEXTURE_2D, IGL::TEXTURE_MAG_FILTER, magFilter->glId));

		CallGLChecked(GLUtil::gl->texParameteri(IGL::TEXTURE_2D, IGL::TEXTURE_WRAP_S, wrapS->glId));
		CallGLChecked(GLUtil::gl->texParameteri(IGL::TEXTURE_2D, IGL::TEXTURE_WRAP_T, wrapT->glId));

		if (borderColorManaged != nullptr) {
			if(IGL::TEXTURE_BORDER_COLOR == 0) {
				// Feature not supported. On android, this must be done in the shader. Currently don't specify this parameter. Maybe in the future just ignore it but not now.
				throw LogicException(LOC);
			}

			float borderColor[] = { borderColorManaged->r, borderColorManaged->g, borderColorManaged->b, borderColorManaged->a };
			CallGLChecked(GLUtil::gl->texParameterfv(IGL::TEXTURE_2D, IGL::TEXTURE_BORDER_COLOR, borderColor));
		}

		// This also allocates the memory and is not used because the above texStorage2D allocates the memory.
		//CallGLChecked(loggedTextureSizeBytes = GLUtil::gl->texImage2D(IGL::TEXTURE_2D, 0, IGL::RGBA8, width, height, 0, IGL::RGBA, IGL::UNSIGNED_BYTE, localBuffer));

		// Data upload is done in the onValidateData because it is dynamic sometimes.
		//if (buffer != nullptr) {
		//	base::Buffer unpackedBuffer = getDataUnpacked(true/*unpackIfNeeded*/, true/*mustExist*/);

		//	if (sizedInternalFormat == IGL::RGBA8) {
		//		// For this to be allowed, a previous glTexImage2D or glTexStorage2D must be called.
		//		CallGLChecked(GLUtil::gl->texSubImage2D(
		//			IGL::TEXTURE_2D/*target*/,
		//			0/*level*/,
		//			0/*xoffset*/,
		//			0/*yoffset*/,
		//			width/*width*/,
		//			height/*height*/,
		//			IGL::RGBA/*format*/,
		//			IGL::UNSIGNED_BYTE/*type*/,
		//			unpackedBuffer.getBlock()/*data*/
		//		));
		//		
		//		dataValidatedOnLoadToGl();
		//	} else {
		//		// Unhandled texture format. Implement required format and type.
		//		throw LogicException(LOC);
		//	}
		//}

		//unbind();
	}
}

void Texture::onUnloadFromGl() {
    if(gHandle != 0) {
		if (sizedInternalFormat == IGL::DEPTH_COMPONENT32F) {
			Stats::vramBytes -= (1/*channels*/ * 4/*bytes_per_channel*/)/*RGBA8*/ * width * height;
		} else {
			// Unchecked format, add a case for it.
			throw LogicException(LOC);
		}

        CallGLChecked(GLUtil::gl->deleteTextures(1, &gHandle, loggedTextureSizeBytes));

        gHandle = 0;
    }

    super::onUnloadFromGl();
}

void Texture::onValidateData() {
	super::onValidateData();

	if (textureArray != nullptr) {
		base::Buffer unpackedBuffer = getDataUnpacked(true/*unpackIfNeeded*/, true/*mustExist*/);

		CallGLChecked(GLUtil::gl->texSubImage3D(
			IGL::TEXTURE_2D_ARRAY/*target*/, 0/*level*/,
			0/*xoffset*/, 0/*yoffset*/, inTextureArrayIndex/*zoffset*/,
			width/*width*/, height/*height*/, 1/*depth*/,
			IGL::RGBA/*format*/, IGL::UNSIGNED_BYTE/*type*/, unpackedBuffer.getBlock()/*data*/
		));
	} else {
		if (buffer != nullptr) {
			base::Buffer unpackedBuffer = getDataUnpacked(true/*unpackIfNeeded*/, true/*mustExist*/);

			if (sizedInternalFormat == IGL::RGBA8) {
				// For this to be allowed, a previous glTexImage2D or glTexStorage2D must be called.
				CallGLChecked(GLUtil::gl->texSubImage2D(
					IGL::TEXTURE_2D/*target*/,
					0/*level*/,
					0/*xoffset*/,
					0/*yoffset*/,
					width/*width*/,
					height/*height*/,
					IGL::RGBA/*format*/,
					IGL::UNSIGNED_BYTE/*type*/,
					unpackedBuffer.getBlock()/*data*/
				));
			} else {
				// Unhandled texture format. Implement required format and type.
				throw LogicException(LOC);
			}
		}
	}
}

void Texture::onBind(int slot, graphics::ShaderProgram* shader) {
	super::onBind(slot, shader);

	if(!GLUtil::gl->getIsGlContext() || textureArray != nullptr) {
		// No gl context or textureArray mode.
		throw LogicException(LOC);
	}

	CallGLChecked(GLUtil::gl->activeTexture(IGL::TEXTURE0 + slot));
	CallGLChecked(GLUtil::gl->bindTexture(IGL::TEXTURE_2D, gHandle));
}

void Texture::onUnbind(int slot, graphics::ShaderProgram* shader) {
	super::onUnbind(slot, shader);

	if(!GLUtil::gl->getIsGlContext() || textureArray != nullptr) {
		// No gl context or textureArray mode.
		throw LogicException(LOC);
    }

    CallGLChecked(GLUtil::gl->activeTexture(IGL::TEXTURE0 + slot));
	CallGLChecked(GLUtil::gl->bindTexture(IGL::TEXTURE_2D, 0));
}

int Texture::getWidth() const {
	return width;
}

int Texture::getHeight() const {
	return height;
}

int Texture::getChannelsCount() const {
	return channelsCount;
}

int Texture::getBitsPerPixel_NOT_BYTES() const {
	return bitsPerPixel_NOT_BYTES;
}

int Texture::getBytesPerPixel_NOT_BITS() const {
	if ((bitsPerPixel_NOT_BYTES % 8) != 0) {
		// The texture bitsPerPixel_NOT_BYTES is not a multiple of 8. Conversion is not valid.
		/// This function should be used only on multiple of 8 bitsPerPixel_NOT_BYTES.
		throw LogicException(LOC);
	}

	return bitsPerPixel_NOT_BYTES / 8;
}

base::Buffer Texture::getData(bool requiredIsDataPacked, bool changePackingIfNeeded, bool mustExist) {
	if (buffer == nullptr) {
		if (mustExist) {
			throw LogicException(LOC);
		}

		return nullptr;
	}

	if (isDataPacked == requiredIsDataPacked) {
		return buffer;
	}

	if (!changePackingIfNeeded) {
		throw LogicException(LOC);
	}

	if (!requiredIsDataPacked) {
		// Do the unpacking.
		int channelsCountOf8BitsPerChannel = 0;
		int w, h;
		unsigned char* unpackedData = stbi_load_from_memory(
			buffer.getBlockUnsignedCharPointer(), buffer.getBytesCount(),
			&w, &h, &channelsCountOf8BitsPerChannel, channelsCount/*desired_channels*/
		);
		if (channelsCountOf8BitsPerChannel != channelsCount) {
			throw LogicException(LOC);
		}

		int unpackedDataBytesCount = width * height * getBytesPerPixel_NOT_BITS();

		base::Buffer unpackedBuffer{ unpackedData, unpackedDataBytesCount, true/*isBufferOwner*/, false/*isBlockMemTracked*/ };

		return unpackedBuffer;
	} else {
		// Do the packing.
		stbi_flip_vertically_on_write(1);
		stbi_write_png_compression_level = 1;
		stbi_write_force_png_filter = 0;

		int packedDataBytesCount = 0;
		unsigned char* packedData = stbi_write_png_to_mem(
			buffer.getBlockUnsignedCharPointer()/*pixels*/, width * getBytesPerPixel_NOT_BITS()/*stride_bytes*/,
			width/*width_of_image*/, height/*height_of_image*/, channelsCount/*number_of_channels_with_8bits_per_channel*/,
			&packedDataBytesCount
		);

		base::Buffer packedBuffer{ packedData, packedDataBytesCount, true/*isBufferOwner*/, false/*isBlockMemTracked*/ };

		return packedBuffer;
	}
}

base::Buffer Texture::getDataUnpacked(bool unpackIfNeeded, bool mustExist) {
	return getData(false/*requiredIsDataPacked*/, unpackIfNeeded, mustExist);
}

base::Buffer Texture::getDataPacked(bool packIfNeeded, bool mustExist) {
	return getData(true/*requiredIsDataPacked*/, packIfNeeded, mustExist);
}

bool Texture::getIsDataPacked() {
	return isDataPacked;
}

void Texture::setIsDataPacked(bool newIsDataPacked) {
	if (this->isDataPacked == newIsDataPacked) {
		return;
	}

	if (buffer != nullptr) {
		buffer = getData(newIsDataPacked/*requiredIsDataPacked*/, true/*changePackingIfNeeded*/, false/*mustExist*/);
	}

	this->isDataPacked = newIsDataPacked;
}

void Texture::disposeMain() {
	Stats::texturesCount--;

	if (buffer != nullptr) {
		Stats::ramBytes -= buffer.getBytesCount();

		buffer = nullptr;
    }
	if (borderColorManaged != nullptr) {
		delete borderColorManaged;
		borderColorManaged = nullptr;
	}

    super::disposeMain();
}

Texture::~Texture() {
	//void
}
