#pragma once

#include <base/gh.h>
#include <base/assets/loader/BasicAssetLoader.h>
#include <string>
#include <graphics/visual/BitmapFont.h>
#include <base/assets/loader/parser/BitmapFontParser.h>
#include <base/assets/loader/util/IAssetLoaderConfig.h>

namespace graphics {
    class TextureArray;
};

class FileSystemBitmapFontLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(FileSystemBitmapFontLoader);
    pub class LoaderConfig : virtual public IAssetLoaderConfig {
        pub bool isCachedPackedGlyphsUsed;

        pub ArrayList<graphics::TextureArray*>* globalTextures;

        pub explicit LoaderConfig(bool isCachedPackedGlyphsUsed, ArrayList<graphics::TextureArray*>* globalTextures)
            : isCachedPackedGlyphsUsed(isCachedPackedGlyphsUsed), globalTextures(globalTextures)
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

    priv BitmapFontParser* parser = nullptr;

    priv base::BitmapFont* data = nullptr;

    pub explicit FileSystemBitmapFontLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath,
        std::shared_ptr<LoaderConfig> config);

    pub virtual int tickStatus(int status, bool isStatusInit) override;

    pub void initStatus_Loading_PreDeps();
    pub void initStatus_Loading_PostDeps();

    pub std::string computeAtlasFolderPath();
    pub virtual void* extractData() override;

    prot void disposeMain() override;
    pub ~FileSystemBitmapFontLoader() override;
};
