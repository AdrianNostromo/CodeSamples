#pragma once

#include <base/gh.h>
#include <base/assets/loader/BasicAssetLoader.h>
#include <string>
#include <base/assets/loader/util/IAssetLoaderConfig.h>
#include <base/audio/sound/Sound.h>
#include <base/audio/IAudio.h>

class FileSystemSoundLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(FileSystemSoundLoader);
    pub class LoaderConfig : virtual public IAssetLoaderConfig {
        //void

        pub explicit LoaderConfig() {
            //void
        }
    };

    pub class LocalStatusCode : public StatusCode {priv typedef StatusCode super;
        pub static const int Loading_PreDeps = super::LAST_USED_INDEX + 1;
        pub static const int Loading_PostDeps = super::LAST_USED_INDEX + 2;
        pub static const int WaitingForDependencies = super::LAST_USED_INDEX + 3;

        prot static const int LAST_USED_INDEX = WaitingForDependencies;

    };
	
    prot base::audio::IAudio* audio;

    priv std::shared_ptr<LoaderConfig> config;

//    asdA11;
//    priv SoundParser* parser = nullptr;

    priv base::audio::Sound* data = nullptr;

    pub explicit FileSystemSoundLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath,
        base::audio::IAudio* audio,
        std::shared_ptr<LoaderConfig> config);

    pub virtual int tickStatus(int status, bool isStatusInit) override;

    pub void initStatus_Loading_PreDeps();
    pub void initStatus_Loading_PostDeps();

    pub std::string computeAtlasFolderPath();
    pub virtual void* extractData() override;

    prot void disposeMain() override;

    pub ~FileSystemSoundLoader() override;
};
