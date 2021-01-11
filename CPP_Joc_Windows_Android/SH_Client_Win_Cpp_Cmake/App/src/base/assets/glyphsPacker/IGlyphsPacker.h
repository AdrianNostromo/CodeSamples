#pragma once

#include <base/gh.h>
#include <memory>
#include <string>
#include <base/memory/SharedPointer.h>
#include <base/list/ArrayList.h>

class TextureRegion;
class Texture;
namespace base {
    class Asset;
};

namespace base {
class IGlyphsPacker {
    pub class ComposedGlyphRegion {
        pub TextureRegion* textureRegion;

        pub std::shared_ptr<base::Asset> textureAsset;

        pub explicit ComposedGlyphRegion(TextureRegion* textureRegion, std::shared_ptr<base::Asset> textureAsset)
            : textureRegion(textureRegion), textureAsset(textureAsset)
        {}
    };
    
    pub virtual sp<ComposedGlyphRegion> getFontTextureRegion(int glyphWidth, int glyphHeight, int padding) = 0;

    pub virtual void saveGlyphsAtlases(ArrayList<Texture*>* usedTexturesArray) = 0;

    pub virtual ~IGlyphsPacker() = default;
};
};
