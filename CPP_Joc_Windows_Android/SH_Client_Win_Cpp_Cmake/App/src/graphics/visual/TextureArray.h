#pragma once

#include <base/gh.h>
#include <base/opengl/util/DataGLResource.h>
#include <base/list/ArrayList.h>
#include "ITextureUnit.h"

// This is the same as a Texture and binds to the same texture slots but has a different target.
// This handles the loading and unloading from gl.
class Texture;

// Don't extend DataGLResource because that validation system should upload all textures, not just the invalid ones and that is not a desired logic.

namespace graphics {
class TextureArray : public DataGLResource, public virtual graphics::ITextureUnit {priv typedef DataGLResource super;pub dCtor(TextureArray);
	pub class Stats {
		pub static int vramBytes;
	};

    // This is used for error checking and texture_index_packing for opengl.
    pub static const int MAX_TEXTURES_COUNT;
    // This is used to set the size of the global tex_arrays array in shaders. It doesn't seem like the best solution but is fine because all of these global tex_arrays are created at start.
    pub static int GLOBAL_TEXTURE_ARRAYS_COUNT;

    // This is used to pack the texture index for shaders to include the tex_array_index and tex_index.
    priv int uIndex;
    
	priv int width = 0;
	priv int height = 0;
	priv int bitsPerPixel_NOT_BYTES = 0;

	pub TextureFilterMin const* minFilter;
	pub TextureFilterMag const* magFilter;
	pub TextureWrap const* wrapS;
	pub TextureWrap const* wrapT;

    priv int minTexturesCapacity;

    // This is used for global texture arrays so they can be bound only once.
    // >= 0; Uses a reserved slot.
    // Note. When calling bind and this is enabled, must use this slot or an error is thrown.
    pub int hardcodedTextureBindSlot;

    // These can have nullptr entries.
    // This has a min capacity and can be increased. Once increased, it only decreases if the last slots are unused (doesn't move textures to other slots).
    priv Array1D<Texture*> texturesList;

    // This is used only for individual textures upload if needed.
    // The DataGLResource logic will force udate all textures datas.
    priv bool isInvalidData = true;

    pub explicit TextureArray(
        int uIndex,
        int width, int height, int bitsPerPixel_NOT_BYTES,
        TextureFilterMin const* minFilter, TextureFilterMag const* magFilter,
        TextureWrap const* wrapS, TextureWrap const* wrapT,
        int minTexturesCapacity, int hardcodedTextureBindSlot);

    pub int getWidth();
    pub int getHeight();

    pub int getBitsPerPixel_NOT_BYTES();
    pub int getBytesPerPixel_NOT_BITS();

    pub bool hookTextureIfCanManage(Texture* texture);
    pub void hookTexture(Texture* texture);
    pub void releaseTexture(Texture* texture);

    prot void onLoadToGl() override;
    prot void onUnloadFromGl() override;

    prot void onValidateData() override;

    pub void updateGLBufferIfNeeded(graphics::ShaderProgram* shader) override;

    pub void onBind(int slot, graphics::ShaderProgram* shader) override;
    pub void onUnbind(int slot, graphics::ShaderProgram* shader) override;

    pub int getBitsPerPixel_NOT_BYTES() const;
    pub int getBytesPerPixel_NOT_BITS() const;

    pub inline int getHardcodedTextureBindSlot() { return hardcodedTextureBindSlot; };

    pub ~TextureArray() override;
};
};
