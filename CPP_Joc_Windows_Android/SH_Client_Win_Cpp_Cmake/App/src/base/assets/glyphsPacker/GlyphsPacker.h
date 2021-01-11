#pragma once

#include <base/gh.h>
#include "GP_Base.h"
#include <base/container/BinPackingBinaryTree.h>
#include <base/list/ArrayList.h>
#include <base/memory/SharedPointer.h>
#include <string>
#include <memory>

class Texture;
namespace base {
    class Asset;
};

namespace base {
// Note. The laoder for this is not a top level asset. There should be no problems with this (maybe sometimes needs to be recreated and re-load the .dat file but that should not occur if multiple assets can load in paralel, even with small bandwidth).
class GlyphsPacker : public GP_Base {priv typedef GP_Base super;pub dCtor(GlyphsPacker);
    pub class PackingTexture {pub dCtor(PackingTexture);
        pub int packingTextureIndex = 0;

        // This is set as a dependency to the fonts. Using this method, fonts unloading also unloads these textures as needed.
        std::shared_ptr<base::Asset> fontDepTextureAsset;

        pub Texture* texture;

        pub sp<BinPackingBinaryTree> glyphsPackingTree;

        pub explicit PackingTexture(int packingTextureIndex, std::shared_ptr<base::Asset> fontDepTextureAsset, Texture* texture, sp<BinPackingBinaryTree> customGlyphsPackingTree);

    };

    priv static std::string CacheId_packingData;

    // Use a list because texture saves must be made only when requested, to avoid incomplete asset writes (eg. only texture and no font info).
    // Only the last entry is the active one, the others are locked and waiting to be saved and removed from the list and removed from the dependencies list.
    priv ArrayList<sp<PackingTexture>> packingTexturesList{};

    pub explicit GlyphsPacker(std::string& cacheId, IAssetsManager* assetsManager, ArrayList<graphics::TextureArray*>* globalTextures, Asset* selfAsset);

    // This is used when the packing texture is loaded from storage. The texture may be nullptr if it was not used.
    pub void initPackingTexture(int packingTextureIndex, sp<BinPackingBinaryTree> customGlyphsPackingTree, std::shared_ptr<Asset> textureAsset);

    pub sp<ComposedGlyphRegion> getFontTextureRegion(int glyphWidth, int glyphHeight, int padding) final;

    priv PackingTexture* getOrCreatePackingTexture();
    priv PackingTexture* createNewPackingTexture(int* override_packingTextureIndex=nullptr, sp<BinPackingBinaryTree> customGlyphsPackingTree=nullptr, std::shared_ptr<Asset> textureAsset=nullptr);

    pub void saveGlyphsAtlases(ArrayList<Texture*>* usedTexturesArray) final;

    priv sp<PackingTexture> getPackingTextureWithTextureIfExists(Texture* tex);

    priv void cachePackingTexture(sp<PackingTexture> packingTexture);

    pub ~GlyphsPacker() override;
};
};
