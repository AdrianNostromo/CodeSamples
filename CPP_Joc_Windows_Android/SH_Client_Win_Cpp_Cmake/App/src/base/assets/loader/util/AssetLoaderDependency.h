#pragma once

#include <base/gh.h>
#include <memory>
#include <base/assets/loader/util/IAssetLoaderConfig.h>
#include <base/assets/util/Asset.h>
#include <base/assets/loader/util/LoadingAssetWrapper.h>
#include "LoaderType.h"

class AssetLoaderDependency {pub dCtor(AssetLoaderDependency);
    pub class DepStatus {
        pub static const int PreRequest = 1;
        pub static const int Loading = 2;
        pub static const int Finished_Success = 3;
        // If this is a cache dependency, it must be cleared from the failed assets list.
        pub static const int Failed_AssetMissingOrInvalid = 4;

    };

    pub std::shared_ptr<AssetIdentifier> assetIdentifier = nullptr;
    pub int fsTarget;

    // Must not already be cached or loading.
    pub std::shared_ptr<IAssetLoaderConfig> loaderConfig = nullptr;

    pub std::string rootFolderOverride;

    pub LoaderType const* loaderTypeOverride = nullptr;

    pub std::shared_ptr<base::Asset> asset = nullptr;

    pub int loadStatus = DepStatus::PreRequest;
    pub std::shared_ptr<base::LoadingAssetWrapper> loadingAsset = nullptr;

    pub explicit AssetLoaderDependency(
        std::shared_ptr<AssetIdentifier> assetIdentifier, int fsTarget,
        std::shared_ptr<IAssetLoaderConfig> loaderConfig,
        std::string &rootFolderOverride);

    pub virtual ~AssetLoaderDependency();
};
