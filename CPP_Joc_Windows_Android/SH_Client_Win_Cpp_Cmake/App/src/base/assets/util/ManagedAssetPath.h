#pragma once

#include <base/gh.h>
#include <string>
#include <base/list/ArrayList.h>
#include <base/assets/util/AssetType.h>

class ManagedAssetPath {pub dCtor(ManagedAssetPath);
    // This is used to make sure that assets are preloaded only for the next preload phase.
    pub static bool isInitialPreloadStarted;

	// Use pointers to avoid calling it's member functions from static calls (when unutialised).
    pub static ArrayList<ManagedAssetPath*>* get_uniqueEntriesList();
    pub static ArrayList<ManagedAssetPath*>* get_entriesToLoadList();

    pub std::string path;

    pub AssetType const* assetType;

    pub static ManagedAssetPath* newPath(std::string path, AssetType const* assetType);

    pub static ManagedAssetPath* stringPathToManagedTextureAtlasPath(std::string path);
    pub static ManagedAssetPath* stringPathToManagedSoundPath(std::string path);
    pub static ManagedAssetPath* stringPathToManagedFontPath(std::string path);

    priv explicit ManagedAssetPath(std::string path, AssetType const* assetType);

    priv static ManagedAssetPath* getManagedAssetPath(std::string path, AssetType const* assetType);

    pub virtual ~ManagedAssetPath();
};
