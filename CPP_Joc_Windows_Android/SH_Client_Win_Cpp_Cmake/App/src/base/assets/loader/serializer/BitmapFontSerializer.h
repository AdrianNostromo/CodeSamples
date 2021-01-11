#pragma once

#include <base/gh.h>
#include <memory>
#include <graphics/visual/BitmapFont.h>
#include <base/assets/util/Asset.h>
#include <sstream>

class IAssetsManager;

namespace base {
class BitmapFontSerializer {
    pub class SerializedData {pub dCtor(SerializedData);
        pub std::shared_ptr<base::Asset> fontAsset;
        pub std::string fontAssetId;
        pub base::BitmapFont* font;
        pub ArrayList<Texture*>* pageTexturesList = nullptr;

        // fntFileContent
        pub std::shared_ptr <std::stringstream> ss = std::make_shared<std::stringstream>();

        pub std::shared_ptr<std::stringstream> fntFileContent = nullptr;

        pub explicit SerializedData(std::shared_ptr<base::Asset> fontAsset, std::string& fontAssetId, base::BitmapFont* font, ArrayList<Texture*>* pageTexturesList);

        pub virtual ~SerializedData();
    };

    pub BitmapFontSerializer() = delete;

    // NOTE. This also converts the base::BitmapFont (bottom left origin) to classic fnt font (top left origin).
    pub static std::shared_ptr<SerializedData> serializeFont(
        std::shared_ptr<base::Asset> fontAsset, std::string& fontAssetId,
        base::BitmapFont* font,
        IAssetsManager* assetsManager);

    pub static void buildSerializedData(
        std::shared_ptr<BitmapFontSerializer::SerializedData> serializedData,
        IAssetsManager* assetsManager);
    pub static void buildSerializedData_general(
        std::shared_ptr<BitmapFontSerializer::SerializedData> serializedData,
        IAssetsManager* assetsManager);

    pub static std::shared_ptr<base::Asset> FindPageTextureAsset(ArrayList<std::shared_ptr<base::Asset>>& dependencyAssets, Texture* pageTexture);

    pub static void buildSerializedData_chars(
        std::shared_ptr<BitmapFontSerializer::SerializedData> serializedData,
        IAssetsManager* assetsManager);
    pub static void buildSerializedData_kernings(
        std::shared_ptr<BitmapFontSerializer::SerializedData> serializedData,
        IAssetsManager* assetsManager);

};
};
