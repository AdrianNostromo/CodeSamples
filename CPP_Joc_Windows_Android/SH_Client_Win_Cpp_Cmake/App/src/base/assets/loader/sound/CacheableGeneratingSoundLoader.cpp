#include "CacheableGeneratingSoundLoader.h"
#include <base/assets/util/AssetType.h>
#include <base/exceptions/AssetMissingOrInvalidException.h>
#include <base/assets/loader/TextureLoader.h>
#include "FileSystemSoundLoader.h"
#include "GeneratedSoundLoader.h"
#include <vendor/stb/stb_image_write.h>
#include <base/fileSystem/Util_FileSystem.h>

CacheableGeneratingSoundLoader::CacheableGeneratingSoundLoader(
    IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier,
    const std::string& assetsFolderPath, std::string cacheFolderPath,
    std::shared_ptr<LoaderConfig> config)
	:super(handler, identifier, assetsFolderPath),
     cacheFolderPath(cacheFolderPath)
{
    if(config != nullptr) {
        this->config = config;
    }else {
        this->config = std::make_shared<LoaderConfig>();
    }
}

int CacheableGeneratingSoundLoader::tickStatus(int status, bool isStatusInit) {
	switch (status) {
    case StatusCode::Iddle: {
        if (!isStatusInit) {
            //void
        }

        return LocalStatusCode::WaitingForDeps_FileSystem;
        //break;
    }
    case LocalStatusCode::WaitingForDeps_FileSystem: {
        if (!isStatusInit) {
            // Start dependency.
            initStatus_WaitingForDeps_FileSystem();
        }else {
            // Wait for dep finish status.
            bool b;
            try {
                b = tickStatus_WaitingForDeps_FileSystem();
            }catch(AssetMissingOrInvalidException& x) {
                std::shared_ptr<AssetLoaderDependency> dep = getDependency(AssetType::Sound, getIdentifier()->id);
                if(dep == nullptr) {
                    throw LogicException(LOC);
                }
                removeDependency(dep);

                return LocalStatusCode::WaitingForDeps_Generator;
            }

            if(b) {
                return LocalStatusCode::Finished_Success;
            }
        }

        break;
    }
    case LocalStatusCode::WaitingForDeps_Generator: {
        if (!isStatusInit) {
            // Start dependency.
            initStatus_WaitingForDeps_Generator();
        }else {
            // Wait for dep finish status.
            bool b;
            try {
                b = tickStatus_WaitingForDeps_Generator();
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

void* CacheableGeneratingSoundLoader::extractData() {
    // This loader type has no data, it is only a controller of other loaders.
    return nullptr;
}

void CacheableGeneratingSoundLoader::initStatus_WaitingForDeps_FileSystem() {
    // Create a dep for the sound from the cache folder.
    std::shared_ptr<FileSystemSoundLoader::LoaderConfig> loaderConfig = std::make_shared<FileSystemSoundLoader::LoaderConfig>();

    std::shared_ptr<AssetLoaderDependency> dep = std::make_shared<AssetLoaderDependency>(
        AssetIdentifier::getOrCreate(AssetType::Sound, getIdentifier()->id), base::Util_FileSystem::FSTarget::Private,
        loaderConfig,
        getCacheFolderPath()
    );

    dep->loaderTypeOverride = LoaderType::FileSystemSound;

    pushDependency(dep);
}

bool CacheableGeneratingSoundLoader::tickStatus_WaitingForDeps_FileSystem() {
    if(!getIsWaitingForDependencies()) {
        std::shared_ptr<AssetLoaderDependency> dep = getDependency(AssetType::Sound, getIdentifier()->id);
        if(dep == nullptr) {
            throw LogicException(LOC);
        }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Failed_AssetMissingOrInvalid) {
            throw AssetMissingOrInvalidException();
        }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Finished_Success) {
            return true;
        }else {
            throw LogicException(LOC);
        }
    }

    return false;
}

void CacheableGeneratingSoundLoader::initStatus_WaitingForDeps_Generator() {
    // Create a dep for the sound from the cache folder.

    std::shared_ptr<GeneratedSoundLoader::LoaderConfig> loaderConfig = std::make_shared<GeneratedSoundLoader::LoaderConfig>();

    std::shared_ptr<AssetLoaderDependency> dep = std::make_shared<AssetLoaderDependency>(
        AssetIdentifier::getOrCreate(AssetType::Sound, getIdentifier()->id), getFsTarget(),
        loaderConfig,
        getAssetsFolderPath()
    );
    dep->loaderTypeOverride = LoaderType::GeneratedSound;

    pushDependency(dep);
}

bool CacheableGeneratingSoundLoader::tickStatus_WaitingForDeps_Generator() {
    if(!getIsWaitingForDependencies()) {
        std::shared_ptr<AssetLoaderDependency> dep = getDependency(AssetType::Sound, getIdentifier()->id);
        if(dep == nullptr) {
            throw LogicException(LOC);
        }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Failed_AssetMissingOrInvalid) {
            throw AssetMissingOrInvalidException();
        }else if(dep->loadStatus == AssetLoaderDependency::DepStatus::Finished_Success) {
            cacheRasterizedSoundAsset(dep->asset);
            return true;
        }else {
            throw LogicException(LOC);
        }
    }

    return false;
}

void CacheableGeneratingSoundLoader::cacheRasterizedSoundAsset(std::shared_ptr<base::Asset> soundAsset) {
//    asdA11;
//    base::audio::Sound *sound = soundAsset->getAsSound();
//
//    // Serialise the sound.
//    std::shared_ptr<base::BitmapSoundSerializer::SerializedData> serializedSoundData = base::BitmapSoundSerializer::serializeSound(
//        soundAsset->getId(),
//        sound,
//        getHandler()
//    );
//
//    writeSerializedSoundData(serializedSoundData, soundAsset->getId());
}

//asdA11;
//void CacheableGeneratingSoundLoader::writeSerializedSoundData(std::shared_ptr<base::BitmapSoundSerializer::SerializedData>& serializedSoundData, std::string& soundAssetId) {
//    writeSerializedSoundData_pageTextures(serializedSoundData);
//    writeSerializedSoundData_fntFile(serializedSoundData, soundAssetId);
//}
//
//void CacheableGeneratingSoundLoader::writeSerializedSoundData_pageTextures(std::shared_ptr<base::BitmapSoundSerializer::SerializedData>& serializedSoundData) {
//    // Save the sound image.
//    if(serializedSoundData->pageTexturesList->size() != 1) {
//        // Currently only 1 page texture is implemented.
//        throw LogicException(LOC);
//    }
//
//    Texture* pageTexture = serializedSoundData->pageTexturesList->getDirect(0);
//
//    // Get the texture asset.
//    std::string& pageTextureAssetId = serializedSoundData->pageTexturesAssetIdList.getReference(0);
//
//    // Generate the save path.
//    std::string fPath_cache_png = getCacheFolderPath() + pageTextureAssetId;
//
//    // Create the subfolders path as needed.
//    std::shared_ptr<IFileHandlerBlocking> fh = Util_FileSystem::fs->getFileHandlerBlocking(fPath_cache_png, Util_FileSystem::FSTarget::Private);
//    fh->deleteIfExists();
//    fh->createBoundedDirectories();
//
//    // Make sure the image is saved as 4 channels (RGBA) png.
//    stbi_flip_vertically_on_write(1);
//    stbi_write_png_compression_level = 1;
//    stbi_write_force_png_filter = 0;
//
//    int pngDataBytesCount;
//    unsigned char* pngData = stbi_write_png_to_mem(
//        pageTexture->peekLocalBuffer(), 0,
//        pageTexture->getWidth(), pageTexture->getHeight(), pageTexture->getBPP(),
//        &pngDataBytesCount
//    );
//
//    base::Buffer pngBuff(pngDataBytesCount, (char*)pngData, true);
//
//    fh->write(pngBuff);
//}
//
//void CacheableGeneratingSoundLoader::writeSerializedSoundData_fntFile(std::shared_ptr<base::BitmapSoundSerializer::SerializedData>& serializedSoundData, std::string& soundAssetId) {
//    // Save the sound fnt file.
//    // Generate the file path.
//    std::string fPath_cache_fnt = getCacheFolderPath() + soundAssetId + ".fnt";
//
//    // Save the file.
//    std::shared_ptr<IFileHandlerBlocking> fh = Util_FileSystem::fs->getFileHandlerBlocking(fPath_cache_fnt, Util_FileSystem::FSTarget::Private);
//    fh->write(serializedSoundData->fntFileContent);
//}

void CacheableGeneratingSoundLoader::disposeMain() {
	//void

    super::disposeMain();
}

CacheableGeneratingSoundLoader::~CacheableGeneratingSoundLoader() {
	//void
}
