#include "AM_Loaders.h"
#include <assert.h>
#include <base/util/StringUtil.h>
#include "loader/TextureLoader.h"
#include "loader/TextLoader.h"
#include "loader/TextureAtlasLoader.h"
#include "loader/GlyphsPackerLoader.h"
#include <base/exceptions/LogicException.h>
#include <base/assets/loader/ImageLoader.h>
#include <base/assets/loader/font/CacheableRasterisingFontLoader.h>
#include <base/assets/loader/font/FileSystemBitmapFontLoader.h>
#include <base/assets/loader/font/RasterisingTrueTypeFontLoader.h>
#include <base/assets/loader/BinaryDataLoader.h>
#include <base/assets/loader/util/LoaderType.h>
#include <base/assets/loader/sound/CacheableGeneratingSoundLoader.h>
#include <base/assets/loader/sound/FileSystemSoundLoader.h>
#include <base/assets/loader/sound/GeneratedSoundLoader.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <base/app/IApp.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>
#include <base/app/config/AppConfig.h>

AM_Loaders::AM_Loaders(IApp* app)
	:super(app)
{
	//void
}

std::shared_ptr<base::LoadingAssetWrapper> AM_Loaders::loadAssetIfNeeded(
    AssetType const* type, std::string& unpreparedId, int fsTarget,
    LoaderType const* loaderTypeOverride,
    std::shared_ptr<IAssetLoaderConfig> loaderConfig, std::string assetsFolderPathOverride,
    std::shared_ptr<base::Asset> superDependantAsset)
{
	std::string id = prepareAssetId(type, unpreparedId);

    if(getAsset(type, id) != nullptr) {
        // Asset already exists.
        return nullptr;
    }

    LoaderType const* loaderType;
    if(loaderTypeOverride != nullptr) {
        loaderType = loaderTypeOverride;
    }else {
        loaderType = LoaderType::getDefaultLoaderForAssetType(type);
    }
    if(loaderType == nullptr) {
        // Unknown loader type or wrong loader for the requested asset type.
        throw LogicException(LOC);
    }

    if(loaderType->defaultLoaderForAssetType == type && loaderType->producedAssetType != type) {
        if(superDependantAsset != nullptr) {
            // Error. This is a controller loader that DOESN'T produce a asset, it cannot have a superDependantAsset;
            throw LogicException(LOC);
        }

        // This is used to activate a loader_controller when the default loader is used. The final asset type will be the correct one assigned by the laoder_controlle.
        type = loaderType->producedAssetType;
    }

    std::shared_ptr<AssetIdentifier> identifier = AssetIdentifier::getOrCreate(type, id);

    bool isLoaderRequired = false;
    std::shared_ptr<base::LoadingAssetWrapper> loadingAsset = getLoadingAsset(type, id);

    bool isTopLevelAsset = (superDependantAsset == nullptr) ? true : false;
    if(loadingAsset == nullptr) {
        // Also check if any controller loader exists.
        std::shared_ptr<base::Asset> asset = std::make_shared<base::Asset>(identifier, isTopLevelAsset);
        loadingAsset = std::make_shared<base::LoadingAssetWrapper>(asset);
        setLoadingAsset(loadingAsset);

        // A loader is required. It may or may not produce a asset.
        isLoaderRequired = true;
    }

    // This is a new or already_loading asset.
    if (superDependantAsset != nullptr) {
        superDependantAsset->connectToSubDependencyAsset(loadingAsset->asset);
    }

    if(!isLoaderRequired) {
        return nullptr;
    }

    // Create a new loader.
    std::string _rootPath = (assetsFolderPathOverride.size() > 0) ? assetsFolderPathOverride : assetsFolderPath;

    activeLoadersCount++;

    BasicAssetLoader* loader = newLoader_uncreated(
        type, id, fsTarget,
        loaderType,
        loaderConfig, _rootPath,
        loadingAsset
    );

    loadingAsset->loader = loader;

    loadersList.appendReference(loader);
    loader->reservedCreate();

	return loadingAsset;
}

BasicAssetLoader* AM_Loaders::newLoader_uncreated(
    AssetType const* type, std::string& id, int fsTarget,
    LoaderType const* loaderType,
    std::shared_ptr<IAssetLoaderConfig> loaderConfig, std::string& rootPath,
    std::shared_ptr<base::LoadingAssetWrapper> loadingAsset)
{
    //GlobalAccessLogHandler::devLogHandler->post_debug("Asset loader creating, Id: %s, type: %i", id.c_str(), type);

    BasicAssetLoader* loader;
    std::shared_ptr<AssetIdentifier> identifier = AssetIdentifier::getOrCreate(type, id);
    if(loaderType == LoaderType::Texture) {
        if (!StringUtil::contains(id, ".png")) {
            throw LogicException(LOC);
        }

        std::shared_ptr<TextureLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig == nullptr) {
            // The olde method was to use a default config but the global tex_arrays that will contain this will required info to select them (eg. minFilter) so there is no default soluton.
            //localisedLoaderConfig = std::make_shared<TextureLoader::LoaderConfig>(
            //    &Texture::TextureFilterMin::Linear/*minFilter*/, &Texture::TextureFilterMag::Linear/*magFilter*/,
            //    &Texture::TextureWrap::ClampToEdge/*wrapS*/, &Texture::TextureWrap::ClampToEdge/*wrapT*/,
            //    globalTextures,
            //    AppConfig::usePackedTextureDataInRam
            //);

            throw LogicException(LOC);
        }

        localisedLoaderConfig = std::dynamic_pointer_cast<TextureLoader::LoaderConfig>(loaderConfig);
        if(localisedLoaderConfig == nullptr) {
            throw LogicException(LOC);
        }

        loader = newt TextureLoader(this, identifier, rootPath, localisedLoaderConfig);
    }else if(loaderType == LoaderType::TextureAtlas) {
        if (!StringUtil::contains(id, ".atlas")) {
            throw LogicException(LOC);
        }

        std::shared_ptr<TextureAtlasLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<TextureAtlasLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = std::make_shared<TextureAtlasLoader::LoaderConfig>(globalTextures);
        }

        loader = newt TextureAtlasLoader(this, identifier, rootPath, localisedLoaderConfig);
    }else if(loaderType == LoaderType::GlyphsPacker) {
        std::shared_ptr<GlyphsPackerLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<GlyphsPackerLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = std::make_shared<GlyphsPackerLoader::LoaderConfig>(globalTextures);
        }

        loader = newt GlyphsPackerLoader(this, identifier, rootPath, cacheFolderPath, localisedLoaderConfig);
    }else if(loaderType == LoaderType::Text) {
        std::shared_ptr<TextLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<TextLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = nullptr;
        }

        loader = newt TextLoader(this, identifier, rootPath, localisedLoaderConfig);
    }else if(loaderType == LoaderType::Image) {
        if (!StringUtil::contains(id, ".png")) {
            throw LogicException(LOC);
        }

        std::shared_ptr<ImageLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<ImageLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = nullptr;
        }

        loader = newt ImageLoader(this, identifier, rootPath, localisedLoaderConfig);
    }else if(loaderType == LoaderType::BinaryData) {
        std::shared_ptr<BinaryDataLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<BinaryDataLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = nullptr;
        }

        loader = newt BinaryDataLoader(this, identifier, rootPath, localisedLoaderConfig);
    }else if(loaderType == LoaderType::CacheableRasterisingFont) {
        std::shared_ptr<CacheableRasterisingFontLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<CacheableRasterisingFontLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = std::make_shared<CacheableRasterisingFontLoader::LoaderConfig>(globalTextures);
        }

        loader = newt CacheableRasterisingFontLoader(this, identifier, rootPath, cacheFolderPath, localisedLoaderConfig);
    }else if(loaderType == LoaderType::FileSystemBitmapFont) {
        std::shared_ptr<FileSystemBitmapFontLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<FileSystemBitmapFontLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            // This asset type does not support auto loading, a configured loader is required.
            throw LogicException(LOC);
            //localisedLoaderConfig = std::make_shared<FileSystemBitmapFontLoader::LoaderConfig>(
            //    true/*isCachedPackedGlyphsUsed*/,
            //    globalTextures
            //);
        }

        loader = newt FileSystemBitmapFontLoader(this, identifier, rootPath, localisedLoaderConfig);
    }else if(loaderType == LoaderType::RasterisingTrueTypeFont) {
        std::shared_ptr<RasterisingTrueTypeFontLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<RasterisingTrueTypeFontLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = std::make_shared<RasterisingTrueTypeFontLoader::LoaderConfig>(globalTextures);
        }

        loader = newt RasterisingTrueTypeFontLoader(
            this, identifier, rootPath,
            localisedLoaderConfig, cacheFolderPath
        );
    }else if(loaderType == LoaderType::CacheableGeneratingSound) {
        std::shared_ptr<CacheableGeneratingSoundLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<CacheableGeneratingSoundLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = nullptr;
        }

        loader = newt CacheableGeneratingSoundLoader(this, identifier, rootPath, cacheFolderPath, localisedLoaderConfig);
    }else if(loaderType == LoaderType::FileSystemSound) {
        std::shared_ptr<FileSystemSoundLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<FileSystemSoundLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = nullptr;
        }

        loader = newt FileSystemSoundLoader(this, identifier, rootPath, app->getAudio(), localisedLoaderConfig);
    }else if(loaderType == LoaderType::GeneratedSound) {
        std::shared_ptr<GeneratedSoundLoader::LoaderConfig> localisedLoaderConfig;
        if (loaderConfig) {
            localisedLoaderConfig = std::dynamic_pointer_cast<GeneratedSoundLoader::LoaderConfig>(loaderConfig);
            if(localisedLoaderConfig == nullptr) {
                throw LogicException(LOC);
            }
        }else {
            localisedLoaderConfig = nullptr;
        }

        loader = newt GeneratedSoundLoader(
            this, identifier, rootPath,
            app->getAudio(),
            localisedLoaderConfig, cacheFolderPath
        );
    }else {
        throw LogicException(LOC);
    }

    loader->initData(loadingAsset, fsTarget);
    loader->reservedCreate();

    return loader;
}

int AM_Loaders::loadAssetsTick() {
    activeLoadersCount = 0;
    failedLoadersCount = 0;
	for (int i = 0; i < loadersList.size(); i++) {
		BasicAssetLoader* loader = *loadersList.getPointer(i);

		if(loader->getStatus() == BasicAssetLoader::StatusCode::Finished_Failed) {
            failedLoadersCount++;
			//GlobalAccessLogHandler::devLogHandler->post_debug("Loader failed (Optional or Mandatory asset). A. Id: %s", loader->getIdentifier()->id.c_str());
		    
			continue;
		}

		if (loader->loadTick() != 0) {
			return -1;
		}

		if (loader->getStatus() == BasicAssetLoader::StatusCode::Finished_Success) {
            // This is a loader that produces and asset, handle it.
            loader->getLoadingAsset()->status = base::LoadingAssetWrapper::Status::Finished_Success;

            loader->getLoadingAsset()->loader = nullptr;
            extractDataFromFinishedLoader(loader);

            loadersList.remove(i);

            loader->reservedDisposeMain();
            delete loader;

            i--;
        } else if (loader->getStatus() == BasicAssetLoader::StatusCode::Finished_Failed) {
		    // Note. This occurs only once. Once a loader is failed, it is not ticked any more.
			//GlobalAccessLogHandler::devLogHandler->post_debug("Loader failed (Optional or Mandatory asset). B. Id: %s", loader->getIdentifier()->id.c_str());
			
            failedLoadersCount++;

            if(loader->getErrorCode() == BasicAssetLoader::ErrorCode::AssetMissingOrInvalid) {
                loader->getLoadingAsset()->status = base::LoadingAssetWrapper::Status::Failed_AssetMissingOrInvalid;
		    }else if(loader->getErrorCode() == BasicAssetLoader::ErrorCode::LogicError) {
                throw LogicException(LOC);
		    }else {
		        // Error code should have a checked value. This should never occur.
                throw LogicException(LOC);
		    }
		} else {
            activeLoadersCount++;

            if (loader->getIsWaitingForDependencies()) {
                bool isAllDepsLoaded = true;

                ArrayList<std::shared_ptr<AssetLoaderDependency>>* loaderDependenciesList = loader->peekLoaderDependenciesList();
                for (int j = 0; j < loaderDependenciesList->size(); j++) {
                    std::shared_ptr<AssetLoaderDependency> dep = loaderDependenciesList->getDirect(j, nullptr);

                    if(dep->loadStatus == AssetLoaderDependency::DepStatus::PreRequest) {
                        std::shared_ptr<base::LoadingAssetWrapper> loadingAsset = loadAssetIfNeeded(
                            dep->assetIdentifier->type, dep->assetIdentifier->id, dep->fsTarget,
                            dep->loaderTypeOverride,
                            dep->loaderConfig, dep->rootFolderOverride,
                            loader->getLoadingAsset()->asset
                        );

                        if (!loadingAsset) {
                            // Asset is already loaded.
                            dep->asset = getAsset(dep->assetIdentifier->type, dep->assetIdentifier->id);
                            if (dep->asset == nullptr) {
                                // The asset should be loaded at this location.
                                throw LogicException(LOC);
                            }

                            dep->loadStatus = AssetLoaderDependency::DepStatus::Finished_Success;
                        } else {
                            // Asset is loading.
                            dep->loadingAsset = loadingAsset;
                            isAllDepsLoaded = false;

                            dep->loadStatus = AssetLoaderDependency::DepStatus::Loading;
                        }
                    }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Loading) {
                        if(dep->loadingAsset->status == base::LoadingAssetWrapper::Status::Loading) {
                            isAllDepsLoaded = false;
                        }else if(dep->loadingAsset->status == base::LoadingAssetWrapper::Status::Finished_Success) {
                            dep->asset = dep->loadingAsset->asset;
                            if (dep->asset == nullptr) {
                                // The asset should be loaded at this location.
                                throw LogicException(LOC);
                            }
                            dep->loadingAsset = nullptr;

                            dep->loadStatus = AssetLoaderDependency::DepStatus::Finished_Success;
                        }else if(dep->loadingAsset->status == base::LoadingAssetWrapper::Status::Failed_AssetMissingOrInvalid) {
                            // Note. The loader will check the status of all the dependencies.
                            //void

                            dep->loadStatus = AssetLoaderDependency::DepStatus::Failed_AssetMissingOrInvalid;
                        }
                    }
                }

                if(isAllDepsLoaded) {
                    loader->clearIsWaitingForDependencies();
                }
            }
		}
	}

	return 0;
}

bool AM_Loaders::isAnyFailedLoaders() {
    if(failedLoadersCount > 0) {
        return true;
    }

    return false;
}

void AM_Loaders::unloadAsset(std::shared_ptr<AssetIdentifier> assetIdentifier) {
    // The asset must exists as loaded or loading.
    std::shared_ptr<base::Asset> asset = getAsset(assetIdentifier->type, assetIdentifier->id);
    std::shared_ptr<base::LoadingAssetWrapper> loadingAsset = nullptr;
    if(asset == nullptr) {
        loadingAsset = getLoadingAsset(assetIdentifier->type, assetIdentifier->id);
        asset = loadingAsset->asset;
    }
    if(asset == nullptr) {
        throw LogicException(LOC);
    }

    // The asset must not have any super_dependants.
    if(asset->getSuperDependantsList()->size() > 0) {
        throw LogicException(LOC);
    }

    // Remove all asset sub_dependencies.
    for(int i=0;i<asset->getSubDependenciesList()->size();i++) {
        std::shared_ptr<AssetIdentifier> dep = asset->getSubDependenciesList()->getDirect(i, nullptr);

        removeAssetDependency(assetIdentifier, dep);
    }

    // If the asset is loading, remove the loader or failed loader.
    // Also remove the loadingAsset from the loading assets map.
    if(loadingAsset != nullptr) {
        // Remove the loader.
        BasicAssetLoader* loader = loadingAsset->loader;
        loadingAsset->loader = nullptr;

        if(!loadersList.remove(loader)) {
            throw LogicException(LOC);
        }

        loader->reservedDisposeMain();
        delete loader;

        // Remove the loading_asset from the loading map.
        if(!removeLoadingAsset(assetIdentifier)) {
            throw LogicException(LOC);
        }
    }else {
        if(!removeAsset(assetIdentifier)) {
            throw LogicException(LOC);
        }
    }
}

void AM_Loaders::removeAssetDependency(
    std::shared_ptr<AssetIdentifier> superDependantAssetIdentifier,
    std::shared_ptr<AssetIdentifier> subDependencyAssetIdentifier)
{
    // Get the asset objects loading or loaded. Both must exist in the loaded or loading map.
    std::shared_ptr<base::Asset> superDependantAsset = getAsset(superDependantAssetIdentifier->type, superDependantAssetIdentifier->id);
    if (superDependantAsset == nullptr) {
        superDependantAsset = getLoadingAssetAsAsset(superDependantAssetIdentifier->type, superDependantAssetIdentifier->id);
    }
    if (superDependantAsset == nullptr) {
        throw LogicException(LOC);
    }

    std::shared_ptr<base::Asset> subDependencyAsset = getAsset(subDependencyAssetIdentifier->type, subDependencyAssetIdentifier->id);
    if(subDependencyAsset == nullptr) {
        subDependencyAsset = getLoadingAssetAsAsset(subDependencyAssetIdentifier->type, subDependencyAssetIdentifier->id);
    }
    if(subDependencyAsset == nullptr) {
        throw LogicException(LOC);
    }

    // Remove the dependancy from both assets.
    if (!superDependantAsset->getSubDependenciesList()->remove(subDependencyAssetIdentifier)) {
        throw LogicException(LOC);
    }
    if (!subDependencyAsset->getSuperDependantsList()->remove(superDependantAssetIdentifier)) {
        throw LogicException(LOC);
    }

    // If the sub asset has not more super_dependants and is not a topLevel asset, remove it and also remove it's loader if any.
    // Note. Use unloadAsset to remove any asset.
    if (!subDependencyAsset->getIsTopLevelAsset() && subDependencyAsset->getSuperDependantsList()->size() == 0) {
        unloadAsset(subDependencyAsset->getIdentifier());
    }
}

void AM_Loaders::extractDataFromFinishedLoader(BasicAssetLoader* loader) {
    // Move asset from loading to loaded.
    std::shared_ptr<base::Asset> asset = loader->getLoadingAsset()->asset;
    if(asset == nullptr) {
        throw LogicException(LOC);
    }
    if (asset->getType()->id >= 0) {
        // This is a actual asset (not container).
        void* data = loader->extractData();
        if(data == nullptr) {
            // This asset type must have a valid data.
            throw LogicException(LOC);
        }

        asset->initData(data);
    }

    if(!removeLoadingAsset(asset->getIdentifier())) {
        throw LogicException(LOC);
    }

    setAsset(asset);

    // Also extract the loader
    while(loader->getNewSubDepsList()->size() > 0) {
        std::shared_ptr<base::Asset> subDepAsset = loader->getNewSubDepsList()->removeAndGetDirect(0);

        // Set dependant before setting the asset.
        asset->connectToSubDependencyAsset(subDepAsset);

        setAsset(subDepAsset);
    }
}

std::shared_ptr<base::LoadingAssetWrapper> AM_Loaders::getLoadingAsset(AssetType const* type, std::string& id) {
    std::shared_ptr<base::LoadingAssetWrapper> loadingAsset = loadingAssetsMap.getDirect(type->id, id, nullptr);

    return loadingAsset;
}

std::shared_ptr<base::Asset> AM_Loaders::getLoadingAssetAsAsset(AssetType const* type, std::string& id) {
    std::shared_ptr<base::LoadingAssetWrapper> loadingAsset = loadingAssetsMap.getDirect(type->id, id, nullptr);
    if(loadingAsset != nullptr) {
        return loadingAsset->asset;
    }

    return nullptr;
}

void AM_Loaders::setLoadingAsset(std::shared_ptr<base::LoadingAssetWrapper> loadingAsset) {
    if (getLoadingAsset(loadingAsset->asset->getType(), loadingAsset->asset->getId())) {
        throw LogicException(LOC);
    }

    AssetType const* type = loadingAsset->asset->getType();
    loadingAssetsMap.putDirect(type->id, loadingAsset->asset->getId(), loadingAsset);
}

bool AM_Loaders::removeLoadingAsset(std::shared_ptr<AssetIdentifier> identifier) {
    bool b = loadingAssetsMap.removeOptional(identifier->type->id, identifier->id);
    if(!b) {
        throw LogicException(LOC);
    }

    return b;
}

void AM_Loaders::disposeMain() {
	for (int i = loadersList.size() - 1; i >= 0; i--) {
		BasicAssetLoader* loader = *loadersList.getPointer(i);

        loadersList.remove(i);

		loader->reservedDisposeMain();
		delete loader;
	}

    super::disposeMain();
}
