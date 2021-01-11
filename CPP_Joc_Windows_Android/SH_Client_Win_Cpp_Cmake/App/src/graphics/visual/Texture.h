#pragma once

#include <base/gh.h>
#include <iostream>
#include <base/opengl/util/DataGLResource.h>
#include <base/list/ArrayList.h>
#include <base/buffer/Buffer.h>
#include "ITextureUnit.h"

class Color;
namespace graphics {
	class TextureArray;
};

class Texture : public DataGLResource, public virtual graphics::ITextureUnit {priv typedef DataGLResource super;pub dCtor(Texture);
	pub class Stats {
		pub static int texturesCount;
		pub static int ramBytes;
		pub static int vramBytes;
	};

	// This is optional. The shadow depth map has this as nullptr because the data exists only in vram for it and so it only has vram allocated and doesn't upload anything.
	priv base::Buffer buffer;

	priv bool isDataPacked;

	priv int width;
	priv int height;

	// Channels may not have the same bits per channel (eg. 3 bytes for depth and 1 byte for stencil is a common opengl texture format).
	priv int channelsCount;
	priv int bitsPerPixel_NOT_BYTES;

	priv unsigned int mipMapLevels;
	// For info about sized internal formats: https://www.khronos.org/registry/OpenGL-Refpages/es3.0/html/glTexStorage2D.xhtml
	priv GLenum sizedInternalFormat;

	pub Texture::TextureFilterMin const* minFilter;
	pub Texture::TextureFilterMag const* magFilter;
	pub Texture::TextureWrap const* wrapS;
	pub Texture::TextureWrap const* wrapT;

	// This is used when Wrap::ClampToBorder is used.
	/// Fetching values outside of the texture range range will use this color.
	pub Color* borderColorManaged = nullptr;

	// This is used as a small optimisation to avoid 1 extra bind in certain situations.
	priv int autoBindSlot;

	// This is handled by the TextureArray;
	pub graphics::TextureArray* textureArray = nullptr;
	pub signed char inTextureArrayIndex = -1;
	// This gets transferred to the shader and the shader can unpack it to get the tex_array index and tex_index_in_tex_array.
	// This is textureArrayIndex * TA::maxTexturesCount + inTextureArrayIndex.
	pub signed char packedTextureIndex = -1;

    pub explicit Texture(
		base::Buffer buffer, bool isDataPacked,
		int width, int height, int channelsCount, int bitsPerPixel_NOT_BYTES,
		unsigned int mipMapLevels, GLenum sizedInternalFormat,
        Texture::TextureFilterMin const* minFilter, Texture::TextureFilterMag const* magFilter,
        Texture::TextureWrap const* wrapS, Texture::TextureWrap const* wrapT,
		ArrayList<graphics::TextureArray*>* globalTextures,
		Color* borderColorManaged = nullptr, int autoBindSlot=0
	);
	prot void createMain() override;

	prot void onLoadToGl() override;
	prot void onUnloadFromGl() override;

	prot void onValidateData() override;

	pub void onBind(int slot, graphics::ShaderProgram* shader) override;
	pub void onUnbind(int slot, graphics::ShaderProgram* shader) override;

	pub int getWidth() const;
	pub int getHeight() const;

	pub int getChannelsCount() const;

	pub int getBitsPerPixel_NOT_BYTES() const;
	pub int getBytesPerPixel_NOT_BITS() const;

	pub base::Buffer getData(bool requiredIsDataPacked, bool changePackingIfNeeded, bool mustExist);
	pub base::Buffer getDataUnpacked(bool unpackIfNeeded, bool mustExist);
	pub base::Buffer getDataPacked(bool packIfNeeded, bool mustExist);

	pub bool getIsDataPacked();
	pub void setIsDataPacked(bool newIsDataPacked);

	prot void disposeMain() override;
	pub ~Texture() override;
};
