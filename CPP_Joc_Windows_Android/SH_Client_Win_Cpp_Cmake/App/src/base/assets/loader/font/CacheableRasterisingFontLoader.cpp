#include "CacheableRasterisingFontLoader.h"
#include <base/assets/util/AssetType.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/assets/loader/TextureLoader.h>
#include "FileSystemBitmapFontLoader.h"
#include "RasterisingTrueTypeFontLoader.h"
#include <vendor/stb/stb_image_write.h>
#include <base/fileSystem/Util_FileSystem.h>
#include <base/assets/loader/util/IAssetLoaderHandler.h>
#include <base/assets/cachesHandler/ICachesGroupsHandler.h>
#include <base/assets/cachesHandler/cacheGroup/ICacheGroup.h>
#include <base/assets/glyphsPacker/IGlyphsPacker.h>

std::string CacheableRasterisingFontLoader::AssetID_glyphs = "glyphs";

CacheableRasterisingFontLoader::CacheableRasterisingFontLoader(
    IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier,
    const std::string& assetsFolderPath, std::string cacheFolderPath,
    std::shared_ptr<LoaderConfig> config)
	:super(handler, identifier, assetsFolderPath),
    config(config),
    cacheFolderPath(cacheFolderPath)
{
    //void
}

int CacheableRasterisingFontLoader::tickStatus(int status, bool isStatusInit) {
	switch (status) {
    case StatusCode::Iddle: {
        if (!isStatusInit) {
            //void
        }

        return LocalStatusCode::WaitingForDeps_FntCache;
        //break;
    }
    case LocalStatusCode::WaitingForDeps_FntCache: {
        if (!isStatusInit) {
			// Make a quick check if the fnt cache exists.
			if (getHandler()->getCacheGroupsHandler()->getCacheGroup(base::ICachesGroupsHandler::CACHE_GROUP_ID_GLOBAL)->isCache(getIdentifier()->id)) {
				isFntCacheEntry = true;

				// Start dependency.
				initStatus_WaitingForDeps_FntCache();
			} else {
				isFntCacheEntry = false;

				// No fnt cache entry exists. Go to the rasteriser directly.
				return LocalStatusCode::WaitingForDeps_TtfAsset;
			}
        }else {
            // Wait for dep finish status.
            bool b;
            try {
                b = tickStatus_WaitingForDeps_FntCache();
            }catch(AssetMissingOrInvalidException& x) {
                std::shared_ptr<AssetLoaderDependency> dep = getDependency(AssetType::Font, getIdentifier()->id);
                if(dep == nullptr) {
                    throw LogicException(LOC);
                }
                removeDependency(dep);

                return LocalStatusCode::WaitingForDeps_TtfAsset;
            }

            if(b) {
                return LocalStatusCode::Finished_Success;
            }
        }

        break;
    }
    case LocalStatusCode::WaitingForDeps_TtfAsset: {
        if (!isStatusInit) {
            // Start dependency.
            initStatus_WaitingForDeps_TtfAsset();
        }else {
            // Wait for dep finish status.
            bool b;
            try {
                b = tickStatus_WaitingForDeps_TtfAsset();
            }catch(AssetMissingOrInvalidException& x) {
                cleanupDependencies();

                throw AssetMissingOrInvalidException();
            }

            if(b) {
                return LocalStatusCode::Finished_Success;
            }
        }

        break;
    }
    default: {
        return -1;
        //break;
    }}

    return status;
}

void* CacheableRasterisingFontLoader::extractData() {
    // This loader type has no data, it is only a controller of other loaders.
    return nullptr;
}

void CacheableRasterisingFontLoader::initStatus_WaitingForDeps_FntCache() {
	// Create a dep for the bitmap font from the cache folder.
    std::shared_ptr<FileSystemBitmapFontLoader::LoaderConfig> loaderConfig = std::make_shared<FileSystemBitmapFontLoader::LoaderConfig>(
        true/*isCachedPackedGlyphsUsed*/,
        config->globalTextures
    );

	std::string depAssetContainerFolderPath = getCacheFolderPath() + base::ICachesGroupsHandler::CACHE_GROUP_ID_GLOBAL + "/";
    std::shared_ptr<AssetLoaderDependency> dep = std::make_shared<AssetLoaderDependency>(
        AssetIdentifier::getOrCreate(AssetType::Font, getIdentifier()->id), base::Util_FileSystem::FSTarget::Private,
        loaderConfig,
		depAssetContainerFolderPath
    );

    dep->loaderTypeOverride = LoaderType::FileSystemBitmapFont;

    pushDependency(dep);
}

bool CacheableRasterisingFontLoader::tickStatus_WaitingForDeps_FntCache() {
    if(!getIsWaitingForDependencies()) {
        std::shared_ptr<AssetLoaderDependency> dep = getDependency(AssetType::Font, getIdentifier()->id);
        if(dep == nullptr) {
            throw LogicException(LOC);
        }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Failed_AssetMissingOrInvalid) {
			// If the font existed in the cache, a cache error occured and need to delete the selected cache index and folder.
			if (isFntCacheEntry) {
				getHandler()->getCacheGroupsHandler()->getCacheGroup(base::ICachesGroupsHandler::CACHE_GROUP_ID_GLOBAL)->cacheIntegrityErrorDetected();
			}

            throw AssetMissingOrInvalidException();
        }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Finished_Success) {
            return true;
        }else {
            throw LogicException(LOC);
        }
    }

    return false;
}

void CacheableRasterisingFontLoader::initStatus_WaitingForDeps_TtfAsset() {
    // Create a dep for the bitmap font from the cache folder.

    std::shared_ptr<RasterisingTrueTypeFontLoader::LoaderConfig> loaderConfig = std::make_shared<RasterisingTrueTypeFontLoader::LoaderConfig>(
        config->globalTextures
    );

    std::shared_ptr<AssetLoaderDependency> dep = std::make_shared<AssetLoaderDependency>(
        AssetIdentifier::getOrCreate(AssetType::Font, getIdentifier()->id), getFsTarget(),
        loaderConfig,
        getAssetsFolderPath()
    );
    dep->loaderTypeOverride = LoaderType::RasterisingTrueTypeFont;

    pushDependency(dep);
}

bool CacheableRasterisingFontLoader::tickStatus_WaitingForDeps_TtfAsset() {
    if(!getIsWaitingForDependencies()) {
        std::shared_ptr<AssetLoaderDependency> dep = getDependency(AssetType::Font, getIdentifier()->id);
        if(dep == nullptr) {
            throw LogicException(LOC);
        }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Failed_AssetMissingOrInvalid) {
			throw AssetMissingOrInvalidException();
        }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Finished_Success) {
            cacheRasterizedFontAsset(dep->asset);
            return true;
        }else {
            throw LogicException(LOC);
        }
    }

    return false;
}

void CacheableRasterisingFontLoader::cacheRasterizedFontAsset(std::shared_ptr<base::Asset> fontAsset) {
    base::BitmapFont *font = fontAsset->getAsFont();

    // Serialise the bitmap font.
    std::shared_ptr<base::BitmapFontSerializer::SerializedData> serializedFontData = base::BitmapFontSerializer::serializeFont(
        fontAsset, fontAsset->getId(),
        font,
        getHandler()
    );
    
    writeSerializedFontData(serializedFontData, fontAsset);
}

void CacheableRasterisingFontLoader::writeSerializedFontData(std::shared_ptr<base::BitmapFontSerializer::SerializedData>& serializedFontData, std::shared_ptr<base::Asset> fontAsset) {
    base::GlyphsPacker* glyphsPackerDep = getHandler()->getGlyphsPacker(AssetID_glyphs);
    if (glyphsPackerDep == nullptr) {
        throw LogicException(LOC);
    }

    glyphsPackerDep->saveGlyphsAtlases(fontAsset->getAsFont()->peekPageTexturesList());
    writeSerializedFontData_fntFile(serializedFontData, fontAsset->getId());

    // This is written to easily check if a font asset exists or not.
    /// This cache entry has no file.
	getHandler()->getCacheGroupsHandler()->getCacheGroup(base::ICachesGroupsHandler::CACHE_GROUP_ID_GLOBAL)
        ->setCache(fontAsset->getId());
}

void CacheableRasterisingFontLoader::writeSerializedFontData_fntFile(std::shared_ptr<base::BitmapFontSerializer::SerializedData>& serializedFontData, std::string& fontAssetId) {
    // Save the font fnt file.
    
    // Save the file.
	std::string s = fontAssetId + ".fnt";
	getHandler()->getCacheGroupsHandler()->getCacheGroup(base::ICachesGroupsHandler::CACHE_GROUP_ID_GLOBAL)
        ->setCache(s, serializedFontData->fntFileContent);
}

void CacheableRasterisingFontLoader::disposeMain() {
	//void

    super::disposeMain();
}

CacheableRasterisingFontLoader::~CacheableRasterisingFontLoader() {
	//void
}
