#include <base/assets/loader/util/LoaderType.h>
#include <base/fileSystem/Util_FileSystem.h>
#include "AM_Preload.h"

AM_Preload::AM_Preload(IApp* app)
	:super(app)
{
	//void
}

void AM_Preload::preloadAsset(AssetType const* type, std::string& path) {
	preloadQueue.append_emplace(type, path);
}

void AM_Preload::preloadAssetB(AssetType const* type, std::string path) {
	preloadQueue.append_emplace(type, path);
}

void AM_Preload::preloadAsset(AssetType const* type, ArrayList<std::string>& pathsList) {
	for (int i = 0; i < pathsList.size(); i++) {
		preloadAsset(type, *pathsList.getPointer(i));
	}
}

void AM_Preload::preloadManagedAssets(ArrayList<ManagedAssetPath*>& entriesToLoadList) {
	for (int i = 0; i < entriesToLoadList.size(); i++) {
        ManagedAssetPath* entry = entriesToLoadList.getDirect(i);

		preloadAsset(entry->assetType, entry->path);
	}
}

bool AM_Preload::isAssetsPreloading() {
	if (preloadQueue.size() > 0 || getActiveLoadersCount() > 0) {
		return true;
	}

	return false;
}

int AM_Preload::preloadTick() {
	while (preloadQueue.size() > 0 && getActiveLoadersCount() < maxParalelLoadersCount) {
		AssetIdentifier& assetIdentifier = *preloadQueue.getPointer(0);

		loadAssetIfNeeded(
		    assetIdentifier.type, assetIdentifier.id, base::Util_FileSystem::FSTarget::EmbeddedAssets,
		    nullptr,
		    nullptr, std::string(), nullptr
		);

		preloadQueue.remove(0);
	}

	if (loadAssetsTick() != 0) {
		return -1;
	}

	return 0;
}
