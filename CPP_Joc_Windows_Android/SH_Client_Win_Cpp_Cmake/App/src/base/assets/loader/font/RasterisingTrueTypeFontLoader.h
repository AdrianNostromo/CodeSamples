#pragma once

#include <base/gh.h>
#include <base/assets/loader/BasicAssetLoader.h>
#include <string>
#include <base/assets/loader/util/IAssetLoaderConfig.h>
#include <vendor/stb/stb_truetype.h>
#include <base/assets/glyphsPacker/IGlyphsPacker.h>

namespace graphics {
    class TextureArray;
};

class RasterisingTrueTypeFontLoader : public BasicAssetLoader {priv typedef BasicAssetLoader super;pub dCtor(RasterisingTrueTypeFontLoader);
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
        pub static const int Rasterising = super::LAST_USED_INDEX + 2;

        prot static const int LAST_USED_INDEX = Rasterising;

    };

    priv static const int MAX_FONT_SIZE = 220;

    priv static std::string AssetID_glyphs;

    priv std::shared_ptr<LoaderConfig> config;

    priv std::string cacheFolderPath;

    priv stbtt_packedchar* packedCharsData = nullptr;
    priv base::Buffer* buff;

    priv base::BitmapFont* data = nullptr;

    priv std::string fontName;
    priv int fontSize = -1;
    priv std::string* charactersToRender = nullptr;

    priv std::shared_ptr<base::Asset> lastGlyphPackingTextureAsset = nullptr;

    pub explicit RasterisingTrueTypeFontLoader(
        IAssetLoaderHandler* handler, std::shared_ptr<AssetIdentifier>& identifier, const std::string& assetsFolderPath,
        std::shared_ptr<LoaderConfig> config, std::string cacheFolderPath);

    pub std::string& getCacheFolderPath() { return cacheFolderPath; }

    pub virtual int tickStatus(int status, bool isStatusInit) override;
    pub void initStatus_loading_preDependencies();
    pub void initStatus_rasterising();
    pub std::string computeAtlasFolderPath();
    pub virtual void* extractData() override;

    priv void writeFontGlyph(
        base::BitmapFont* fontAsset, base::IGlyphsPacker* glyphsPacker,
        int unicodeCodepointDecimal, stbtt_packedchar* packedCharData,
        unsigned char* glyphixels, int glyphX, int glyphY, int glyphWidth, int glyphHeight, int padding);

    priv void packFontBitmap(int fontSize, std::string& charactersToRender, char* ttf_buffer, 
        base::IGlyphsPacker* glyphsPacker, base::BitmapFont* fontAsset);

    priv static void copyGlyph1CHannelTo4ChannelsFlipY(
        unsigned char* inPixels1Channel, int inImageW, int inRegionX, int inRegionY,
        unsigned char* outPixels4Channels, int outImageW, int outImageH, int outRegionX, int outRegionY,
        int regionW, int regionH);

    pub void cleanup();

    prot void disposeMain() override;
    pub ~RasterisingTrueTypeFontLoader() override;
};
