#pragma once

#include <base/gh.h>
#include <base/assets/loader/BasicAssetLoader.h>
#include <string>
#include <base/assets/loader/util/IAssetLoaderConfig.h>
#include <vendor/stb/stb_truetype.h>
#include <base/audio/sound/Sound.h>
#include <base/audio/IAudio.h>

class GeneratedSoundLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(GeneratedSoundLoader);
    pub class LoaderConfig : virtual public IAssetLoaderConfig {
        //void

        pub explicit LoaderConfig() {
            //void
        }
    };

    pub class LocalStatusCode : public StatusCode {priv typedef StatusCode super;
        pub static const int Loading_PreDeps = super::LAST_USED_INDEX + 1;

        prot static const int LAST_USED_INDEX = Loading_PreDeps;

    };

	prot base::audio::IAudio* audio;

    priv std::shared_ptr<LoaderConfig> config;

    priv std::string cacheFolderPath;

    priv unsigned char* temp_bitmap_1Channel = nullptr;
    priv unsigned char* temp_bitmap_4Channels = nullptr;
    priv stbtt_packedchar* pData = nullptr;
    priv base::Buffer* buff;

    priv base::audio::Sound* data = nullptr;

    pub explicit GeneratedSoundLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath,
        base::audio::IAudio* audio,
        std::shared_ptr<LoaderConfig> config, std::string cacheFolderPath);

    pub std::string& getCacheFolderPath() { return cacheFolderPath; }

    pub virtual int tickStatus(int status, bool isStatusInit) override;
    pub void initStatus_loading_preDependencies();
	
    pub virtual void* extractData() override;

    prot void disposeMain() override;

    pub void cleanup();
    pub ~GeneratedSoundLoader() override;
};
