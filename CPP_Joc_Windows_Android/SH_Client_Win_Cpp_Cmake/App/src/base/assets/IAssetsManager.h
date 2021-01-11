#pragma once

#include <base/gh.h>
#include "IFontProvider.h"
#include <graphics/visual/Texture.h>
#include <graphics/visual/TextureRegion.h>
#include <graphics/visual/TextureAtlas.h>
#include <graphics/visual/TextureRegion.h>
#include <base/assets/glyphsPacker/GlyphsPacker.h>
#include <memory>
#include <base/list/ArrayList.h>
#include <base/buffer/Buffer.h>
#include <graphics/visual/Image.h>
#include <graphics/visual/BitmapFont.h>
#include <base/assets/util/Asset.h>
#include <base/assets/util/AssetIdentifier.h>
#include <base/assets/util/AssetType.h>

namespace base {
	class ICachesGroupsHandler;
	class IGlyphsPacker;
};

class IAssetsManager : virtual public base::IFontProvider {
	pub virtual std::shared_ptr<base::Asset> getAsset(AssetIdentifier& assetIdentifier) = 0;
	pub virtual std::shared_ptr<base::Asset> getAsset(AssetType const* type, std::string& id) = 0;

	pub virtual void setAsset(std::shared_ptr<base::Asset> asset) = 0;

	pub virtual Texture* getTexture(std::string& textureID) = 0;
	pub virtual base::String* getText(std::string& textID) = 0;
	pub virtual base::Buffer* getBinaryData(std::string& id) = 0;
	pub virtual base::Image* getImage(std::string& id) = 0;
	pub virtual base::BitmapFont* getFont(std::string& assetId) = 0;
	pub virtual base::audio::Sound* getSound(std::string& assetId) = 0;
	pub virtual TextureAtlas* getTextureAtlas(std::string& atlasID) = 0;
	pub virtual base::GlyphsPacker* getGlyphsPacker(std::string& assetId) = 0;
	pub virtual TextureRegion* getTextureAtlasRegion(std::string& atlasID, std::string& regionID) = 0;
	/*
	 * regionIndiceStart: manual or use -1 for automatic start index (0 or 1);
	 * regionIndiceEnd: manual or use -1 for automatic end index;
	 */
	pub virtual std::shared_ptr<ArrayList<TextureRegion*>> getTextureAtlasRegionsArray(
	    std::string& atlasID,
	    std::string& regionIDPrefix, std::string& regionIDSuffix, int regionIndiceStart, int regionIndiceEnd, int indiceZeroPadding) = 0;
	
	pub virtual base::ICachesGroupsHandler* getCacheGroupsHandler() = 0;

	pub virtual void removeAssetDependency(
		std::shared_ptr<AssetIdentifier> superDependantAssetIdentifier,
		std::shared_ptr<AssetIdentifier> subDependencyAssetIdentifier) = 0;

	pub virtual ~IAssetsManager() = default;
};
