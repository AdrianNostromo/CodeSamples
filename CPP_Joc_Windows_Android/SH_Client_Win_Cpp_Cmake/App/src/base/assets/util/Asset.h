#pragma once

#include <base/gh.h>
#include <graphics/visual/Texture.h>
#include <graphics/visual/TextureAtlas.h>
#include <graphics/visual/BitmapFont.h>
#include <base/buffer/Buffer.h>
#include <graphics/visual/Image.h>
#include <base/list/ArrayList.h>
#include "AssetIdentifier.h"
#include <base/string/String.h>
#include <base/audio/sound/Sound.h>

namespace base {
    class GlyphsPacker;
};

namespace base {
class Asset {pub dCtor(Asset);
    priv std::shared_ptr<AssetIdentifier> identifier;

    // true: Asset is requested manually. It may also be a dependancy but it won't be removed if the last dependant it has is removed.
    priv bool isTopLevelAsset = false;
    priv ArrayList<std::shared_ptr<AssetIdentifier>> subDependenciesList;
    priv ArrayList<std::shared_ptr<AssetIdentifier>> superDependantsList;

    priv void* data = nullptr;

    pub explicit Asset(std::shared_ptr<AssetIdentifier>& identifier, bool isTopLevelAsset);

    pub void initData(void* data);

    pub std::shared_ptr<AssetIdentifier> getIdentifier() { return identifier; }
    pub AssetType const* getType() { return identifier->type; }
    pub std::string& getId() { return identifier->id; }

    pub bool getIsTopLevelAsset();

    pub ArrayList<std::shared_ptr<AssetIdentifier>>* getSubDependenciesList();
    pub ArrayList<std::shared_ptr<AssetIdentifier>>* getSuperDependantsList();

    pub std::shared_ptr<AssetIdentifier> getSingleSubDependencYWithType(AssetType const* assetType, bool mustExist);

    pub void connectToSubDependencyAsset(std::shared_ptr<base::Asset> subDependencyAsset);

    pub Texture* getAsTexture();
    pub TextureAtlas* getAsTextureAtlas();
    pub base::GlyphsPacker* getAsGlyphsPacker();
    pub base::String* getAsText();
    pub base::Buffer* getAsBuffer();
    pub base::Image* getAsImage();
    pub base::BitmapFont* getAsFont();
    pub base::audio::Sound* getAsSound();

    pub virtual ~Asset();
};
};
