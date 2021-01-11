#include "ManagedAssetPath.h"
#include "AssetType.h"
#include <base/exceptions/LogicException.h>
#include <base/MM.h>

bool ManagedAssetPath::isInitialPreloadStarted = false;

ArrayList<ManagedAssetPath*>* ManagedAssetPath::get_uniqueEntriesList() {
	static ArrayList<ManagedAssetPath*>* uniqueEntriesList = new ArrayList<ManagedAssetPath*>();

	return uniqueEntriesList;
}
ArrayList<ManagedAssetPath*>* ManagedAssetPath::get_entriesToLoadList() {
	static ArrayList<ManagedAssetPath*>* entriesToLoadList = new ArrayList<ManagedAssetPath*>();

	return entriesToLoadList;
}

ManagedAssetPath::ManagedAssetPath(std::string path, AssetType const* assetType)
    : path(path), assetType(assetType)
{
    //void
}

ManagedAssetPath* ManagedAssetPath::newPath(std::string path, AssetType const* assetType) {
    if(isInitialPreloadStarted) {
        throw LogicException(LOC);
    }
	if(get_uniqueEntriesList() == nullptr || get_entriesToLoadList() == nullptr) {
        throw LogicException(LOC);
    }

    ManagedAssetPath* managedAssetPath = getManagedAssetPath(path, assetType);

    if(managedAssetPath == nullptr) {
        get_uniqueEntriesList()->appendDirect(managedAssetPath = new ManagedAssetPath(path, assetType));
        get_entriesToLoadList()->appendDirect(managedAssetPath);
    }

    return managedAssetPath;
}

ManagedAssetPath* ManagedAssetPath::getManagedAssetPath(std::string path, AssetType const* assetType) {
	ArrayList<ManagedAssetPath*>* uniqueEntriesList = get_uniqueEntriesList();

    for(int i=0; i<uniqueEntriesList->size(); i++) {
        ManagedAssetPath* entry = uniqueEntriesList->getDirect(i);

        if(entry->assetType == assetType && entry->path == path) {
            return entry;
        }
    }

    return nullptr;
}

ManagedAssetPath* ManagedAssetPath::stringPathToManagedTextureAtlasPath(std::string path) {
    return newPath(path, AssetType::TextureAtlas);
}

ManagedAssetPath* ManagedAssetPath::stringPathToManagedSoundPath(std::string path) {
    return newPath(path, AssetType::Sound);
}

ManagedAssetPath* ManagedAssetPath::stringPathToManagedFontPath(std::string path) {
    return newPath(path, AssetType::Font);
}

ManagedAssetPath::~ManagedAssetPath() {
    //void
}
