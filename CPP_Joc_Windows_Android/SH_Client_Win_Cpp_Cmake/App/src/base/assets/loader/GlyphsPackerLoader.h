#pragma once

#include <base/gh.h>
#include "BasicAssetLoader.h"
#include <string>
#include <base/assets/glyphsPacker/GlyphsPacker.h>
#include <base/assets/loader/util/IAssetLoaderConfig.h>
#include <base/buffer/StreamBuffer.h>

class IEventListener;
namespace base {
    class AsyncFHEvent;
    class IFileHandlerAsync;
};

class GlyphsPackerLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(GlyphsPackerLoader);
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

    priv static std::string CacheId_packingData;

    priv std::shared_ptr<LoaderConfig> config;

    priv std::string cacheFolderPath;

    priv base::GlyphsPacker* data = nullptr;

    priv int packingTextureIndex = -1;
    priv sp<BinPackingBinaryTree> binPackingStructure = nullptr;
    priv std::shared_ptr<AssetLoaderDependency> textureDep = nullptr;

    priv bool isAsyncReadInProgress = false;
    priv std::shared_ptr<base::IFileHandlerAsync> fhAsyncRead = nullptr;
    priv ArrayList<std::shared_ptr<IEventListener>> fhReadListenersList{};
    priv base::Buffer asyncReadBuff = nullptr;

    pub explicit GlyphsPackerLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath, std::string cacheFolderPath,
        std::shared_ptr<LoaderConfig> config);
    
    prot std::string& getCacheFolderPath();

    pub virtual int tickStatus(int status, bool isStatusInit) override;

    pub void initStatus_loading_preDependencies();
    priv void onAsyncFHEvent(IEventListener& eventListener, base::AsyncFHEvent& event);
    pub bool tickStatus_loadingPredeps();
    
    pub std::string computeAtlasFolderPath();
    pub void initStatus_loading_postDependencies();
    pub virtual void* extractData() override;

    prot void disposeMain() override;
    pub ~GlyphsPackerLoader() override;
};
