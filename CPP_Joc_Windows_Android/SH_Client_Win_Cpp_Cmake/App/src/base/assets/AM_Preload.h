#pragma once

#include <base/assets/AM_CachesHandler.h>
#include "util/AssetIdentifier.h"
#include <string>
#include <base/list/ArrayList.h>
#include <base/assets/util/ManagedAssetPath.h>

class AM_Preload : public base::AM_CachesHandler {
private: typedef base::AM_CachesHandler super;
private:
	static const int maxParalelLoadersCount = 1;

	ArrayList<AssetIdentifier> preloadQueue;
public:
	AM_Preload(AM_Preload const&) = delete;
	AM_Preload(AM_Preload &&) = default;
	AM_Preload& operator=(AM_Preload const&) = delete;
	AM_Preload& operator=(AM_Preload &&) = default;

	explicit AM_Preload(IApp* app);

	void preloadAsset(AssetType const* type, std::string& path);
	void preloadAssetB(AssetType const* type, std::string path);
	void preloadAsset(AssetType const* type, ArrayList<std::string>& pathsList);
	void preloadManagedAssets(ArrayList<ManagedAssetPath*>& entriesToLoadList);
	bool isAssetsPreloading();
	int preloadTick();
};
