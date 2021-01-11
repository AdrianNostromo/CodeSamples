#include "AssetLoaderDependency.h"

AssetLoaderDependency::AssetLoaderDependency(
    std::shared_ptr<AssetIdentifier> assetIdentifier, int fsTarget,
    std::shared_ptr<IAssetLoaderConfig> loaderConfig,
    std::string& rootFolderOverride)
    : assetIdentifier(assetIdentifier), fsTarget(fsTarget),
      loaderConfig(loaderConfig),
      rootFolderOverride(rootFolderOverride)
{
	//void
}

AssetLoaderDependency::~AssetLoaderDependency() {
	//void
}
