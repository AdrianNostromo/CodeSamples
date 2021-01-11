#pragma once

#include <base/gh.h>
#include <base/assets/AM_Base.h>
#include <base/map/Map2D.h>
#include <string>
#include <base/assets/util/Asset.h>
#include <base/opengl/IGL.h>

class AM_AssetsMap : public AM_Base {priv typedef AM_Base super;pub dCtor(AM_AssetsMap);
	priv Map2D<int, std::string, std::shared_ptr<base::Asset>> assetsMap{};

	pub explicit AM_AssetsMap(IApp* app);
	prot void createMain() override;

	pub std::shared_ptr<base::Asset> getAsset(AssetType const* type, std::string& id) override;
	pub std::shared_ptr<base::Asset> getAsset(AssetIdentifier& assetIdentifier) override;

	pub std::string prepareAssetId(AssetType const* type, std::string& id);
	pub void setAsset(std::shared_ptr<base::Asset> asset) final;
	// Call unloadAsset to remvoe a asset.
	prot bool removeAsset(std::shared_ptr<AssetIdentifier> identifier);

	pub Texture* getTexture(std::string& textureID) override;
	pub base::String* getText(std::string& textID) override;
	pub base::Buffer* getBinaryData(std::string& textID) override;
	pub base::Image* getImage(std::string& textID) override;
	pub base::BitmapFont* getFont(std::string& assetId) override;
	pub base::audio::Sound* getSound(std::string& assetId) override;
	pub TextureAtlas* getTextureAtlas(std::string& atlasID) override;
	pub base::GlyphsPacker* getGlyphsPacker(std::string& assetId) override;
	pub TextureRegion* getTextureAtlasRegion(std::string& atlasID, std::string& regionID) override;
	pub std::shared_ptr<ArrayList<TextureRegion*>> getTextureAtlasRegionsArray(
        std::string& atlasID,
        std::string& regionIDPrefix, std::string& regionIDSuffix, int regionIndiceStart, int regionIndiceEnd, int indiceZeroPadding) override;

	pub base::BitmapFont* getFont(
		std::string& fontName, int fontSize, int fontWeight, std::string& charactersRange,
		bool retNullOnBadParams, bool mustExits) final;

	prot void disposeMain() override;
	pub ~AM_AssetsMap() override = default;
};
