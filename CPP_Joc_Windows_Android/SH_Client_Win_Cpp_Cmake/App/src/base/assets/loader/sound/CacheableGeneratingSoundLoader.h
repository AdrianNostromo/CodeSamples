#pragma once

#include <base/gh.h>
#include <base/assets/loader/BasicAssetLoader.h>
#include <string>
#include <base/assets/loader/util/IAssetLoaderConfig.h>

class CacheableGeneratingSoundLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(CacheableGeneratingSoundLoader);
    pub class LoaderConfig : virtual public IAssetLoaderConfig {
        //void

        pub explicit LoaderConfig() {
            //void
        }
    };

    pub class LocalStatusCode : public StatusCode {priv typedef StatusCode super;
        pub static const int WaitingForDeps_FileSystem = super::LAST_USED_INDEX + 1;
        pub static const int WaitingForDeps_Generator = super::LAST_USED_INDEX + 2;

        prot static const int LAST_USED_INDEX = WaitingForDeps_Generator;

    };

    priv std::shared_ptr<LoaderConfig> config;

    priv std::string cacheFolderPath;

    pub explicit CacheableGeneratingSoundLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath, std::string cacheFolderPath,
        std::shared_ptr<LoaderConfig> config);

    pub std::string& getCacheFolderPath() { return cacheFolderPath; }

    pub int tickStatus(int status, bool isStatusInit) override;
    pub void* extractData() override;

    priv void initStatus_WaitingForDeps_FileSystem();
    priv bool tickStatus_WaitingForDeps_FileSystem();

    priv void initStatus_WaitingForDeps_Generator();
    priv bool tickStatus_WaitingForDeps_Generator();

    priv void cacheRasterizedSoundAsset(std::shared_ptr<base::Asset> soundAsset);

    prot void disposeMain() override;
    pub ~CacheableGeneratingSoundLoader() override;
    
//    asdA11;
//    void writeSerializedSoundData(std::shared_ptr<base::BitmapSoundSerializer::SerializedData>& serializedSoundData, std::string& soundAssetId);
//    void writeSerializedSoundData_pageTextures(std::shared_ptr<base::BitmapSoundSerializer::SerializedData>& serializedSoundData);
//    void writeSerializedSoundData_fntFile(std::shared_ptr<base::BitmapSoundSerializer::SerializedData>& serializedSoundData, std::string& soundAssetId);

};
