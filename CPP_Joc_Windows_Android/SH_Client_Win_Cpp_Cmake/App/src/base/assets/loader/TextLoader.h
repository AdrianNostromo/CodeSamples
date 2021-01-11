#pragma once

#include <base/gh.h>
#include "BasicAssetLoader.h"
#include <string>
#include <base/assets/loader/util/IAssetLoaderConfig.h>

class TextLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(TextLoader);
	pub class LoaderConfig : virtual public IAssetLoaderConfig {
        pub explicit LoaderConfig() {
			//void
		}
	};

    pub class LocalStatusCode : public StatusCode {priv typedef StatusCode super;
        pub static const int Loading = super::LAST_USED_INDEX + 1;

        prot static const int LAST_USED_INDEX = Loading;

    };

    priv std::shared_ptr<LoaderConfig> config;

    priv base::String* data = nullptr;

    pub explicit TextLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath,
        std::shared_ptr<LoaderConfig> config);

    pub virtual int tickStatus(int status, bool isStatusInit) override;
    pub virtual void initStatus_loading();
    pub virtual void* extractData() override;

    prot void disposeMain() override;
    pub ~TextLoader() override;
};
