#pragma once

#include <base/gh.h>
#include <base/assets/AM_AssetsMap.h>
#include "loader/BasicAssetLoader.h"
#include <base/list/ArrayList.h>
#include <memory>
#include <base/assets/loader/util/IAssetLoaderConfig.h>
#include <base/assets/loader/util/IAssetLoaderHandler.h>

class AM_Loaders : public AM_AssetsMap, virtual public IAssetLoaderHandler {priv typedef AM_AssetsMap super;pub dCtor(AM_Loaders);
    priv ArrayList<BasicAssetLoader*> loadersList{};
    // Use this to easily detect load errors.
    priv int activeLoadersCount = 0;
    priv int failedLoadersCount = 0;

    priv Map2D<int, std::string, std::shared_ptr<base::LoadingAssetWrapper>> loadingAssetsMap{};

	pub explicit AM_Loaders(IApp* app);

    pub int getActiveLoadersCount() { return activeLoadersCount; }

    // true; Started loading.
    // false; Already cached or loading.
    // superDependant==nullptr; The asset is made top level even if it exists.
    pub std::shared_ptr<base::LoadingAssetWrapper> loadAssetIfNeeded(
        AssetType const* type, std::string& unpreparedId, int fsTarget,
        LoaderType const* loaderTypeOverride,
        std::shared_ptr<IAssetLoaderConfig> loaderConfig, std::string assetsFolderPathOverride,
        std::shared_ptr<base::Asset> superDependantAsset);

    pub int loadAssetsTick();
    pub bool isAnyFailedLoaders();

    pub void unloadAsset(std::shared_ptr<AssetIdentifier> assetIdentifier);
    pub void removeAssetDependency(
        std::shared_ptr<AssetIdentifier> superDependantAssetIdentifier,
        std::shared_ptr<AssetIdentifier> subDependencyAssetIdentifier) override;

    priv void extractDataFromFinishedLoader(BasicAssetLoader* loader);

    priv std::shared_ptr<base::LoadingAssetWrapper> getLoadingAsset(AssetType const* type, std::string& id);
    priv std::shared_ptr<base::Asset> getLoadingAssetAsAsset(AssetType const* type, std::string& id);
    priv void setLoadingAsset(std::shared_ptr<base::LoadingAssetWrapper> asset);
    priv bool removeLoadingAsset(std::shared_ptr<AssetIdentifier> identifier);

    priv BasicAssetLoader* newLoader_uncreated(
        AssetType const* assetType, std::string& id, int fsTarget,
        LoaderType const* loaderType,
        std::shared_ptr<IAssetLoaderConfig> loaderConfig, std::string& rootPath,
        std::shared_ptr<base::LoadingAssetWrapper> loadingAsset);

    prot void disposeMain() override;
};
