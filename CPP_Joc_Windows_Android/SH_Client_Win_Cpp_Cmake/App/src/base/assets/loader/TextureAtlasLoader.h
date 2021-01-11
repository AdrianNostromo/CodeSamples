#pragma once

#include <base/gh.h>
#include "BasicAssetLoader.h"
#include <string>
#include <graphics/visual/TextureAtlas.h>
#include <base/assets/loader/parser/TextureAtlasParser.h>
#include <base/assets/loader/util/IAssetLoaderConfig.h>

namespace graphics {
    class TextureArray;
};

class TextureAtlasLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(TextureAtlasLoader);
    pub class LoaderConfig : virtual public IAssetLoaderConfig {
        pub ArrayList<graphics::TextureArray*>* globalTextures;

        pub explicit LoaderConfig(ArrayList<graphics::TextureArray*>* globalTextures)
            : globalTextures(globalTextures)
        {
            //void
        }
    };

    pub class LocalStatusCode : public StatusCode {priv typedef StatusCode super;
        pub static const int Loading_PreDeps = super::LAST_USED_INDEX + 1;
        pub static const int Loading_PostDeps = super::LAST_USED_INDEX + 2;
        pub static const int WaitingForDependencies = super::LAST_USED_INDEX + 3;

        prot static const int LAST_USED_INDEX = WaitingForDependencies;

    };

    priv std::shared_ptr<LoaderConfig> config;

    priv TextureAtlasParser* parser = nullptr;

    priv TextureAtlas* data = nullptr;

    pub explicit TextureAtlasLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath,
        std::shared_ptr<LoaderConfig> config);

    pub virtual int tickStatus(int status, bool isStatusInit) override;
    pub void initStatus_loading_preDependencies();
    pub std::string computeAtlasFolderPath();
    pub void initStatus_loading_postDependencies();
    pub virtual void* extractData() override;

    prot void disposeMain() override;
    pub ~TextureAtlasLoader() override;
};
