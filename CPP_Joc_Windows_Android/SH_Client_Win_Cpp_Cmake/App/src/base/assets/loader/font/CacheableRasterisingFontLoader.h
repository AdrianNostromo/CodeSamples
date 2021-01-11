#pragma once

#include <base/gh.h>
#include <base/assets/loader/BasicAssetLoader.h>
#include <string>
#include <base/assets/loader/util/IAssetLoaderConfig.h>
#include <base/assets/loader/serializer/BitmapFontSerializer.h>

namespace graphics {
    class TextureArray;
};

class CacheableRasterisingFontLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(CacheableRasterisingFontLoader);
    pub class LoaderConfig : virtual public IAssetLoaderConfig {
        pub ArrayList<graphics::TextureArray*>* globalTextures;

        pub explicit LoaderConfig(ArrayList<graphics::TextureArray*>* globalTextures)
            : globalTextures(globalTextures)
        {
            //void
        }
    };

    pub class LocalStatusCode : public StatusCode {priv typedef StatusCode super;
        pub static const int WaitingForDeps_FntCache = super::LAST_USED_INDEX + 1;
        pub static const int WaitingForDeps_TtfAsset = super::LAST_USED_INDEX + 2;

        prot static const int LAST_USED_INDEX = WaitingForDeps_TtfAsset;

    };

    priv static std::string AssetID_glyphs;

    priv std::shared_ptr<LoaderConfig> config;

    priv std::string cacheFolderPath;

    priv bool isFntCacheEntry = false;

    pub explicit CacheableRasterisingFontLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath, std::string cacheFolderPath,
        std::shared_ptr<LoaderConfig> config);

    pub std::string& getCacheFolderPath() { return cacheFolderPath; }

    pub int tickStatus(int status, bool isStatusInit) override;
    pub void* extractData() override;

    priv void initStatus_WaitingForDeps_FntCache();
    priv bool tickStatus_WaitingForDeps_FntCache();

    priv void initStatus_WaitingForDeps_TtfAsset();
    priv bool tickStatus_WaitingForDeps_TtfAsset();

    priv void cacheRasterizedFontAsset(std::shared_ptr<base::Asset> fontAsset);

    priv void writeSerializedFontData(std::shared_ptr<base::BitmapFontSerializer::SerializedData>& serializedFontData, std::shared_ptr<base::Asset> fontAsset);
    priv void writeSerializedFontData_fntFile(std::shared_ptr<base::BitmapFontSerializer::SerializedData>& serializedFontData, std::string& fontAssetId);

    prot void disposeMain() override;
    pub ~CacheableRasterisingFontLoader() override;
};
