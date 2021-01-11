#include "BitmapFontSerializer.h"
#include <base/util/StringUtil.h>
#include <base/math/Math.h>
#include <base/assets/util/AssetType.h>
#include <base/assets/IAssetsManager.h>

using namespace base;

BitmapFontSerializer::SerializedData::SerializedData(std::shared_ptr<base::Asset> fontAsset, std::string& fontAssetId, base::BitmapFont* font, ArrayList<Texture*>* pageTexturesList)
    : fontAsset(fontAsset), fontAssetId(fontAssetId), font(font), pageTexturesList(pageTexturesList)
{
    //void
}

BitmapFontSerializer::SerializedData::~SerializedData() {
    //void
}

std::shared_ptr<BitmapFontSerializer::SerializedData> BitmapFontSerializer::serializeFont(
    std::shared_ptr<base::Asset> fontAsset, std::string& fontAssetId,
    base::BitmapFont* font,
    IAssetsManager* assetsManager)
{
    std::shared_ptr<BitmapFontSerializer::SerializedData> serializedData = std::make_shared<BitmapFontSerializer::SerializedData>(
        fontAsset, fontAssetId,
        font,
        font->peekPageTexturesList()
    );

    // NOTE. This also converts the base::BitmapFont (bottom left origin) to classic fnt font (top left origin).
    std::stringstream ss{};

    buildSerializedData(serializedData, assetsManager);

    serializedData->fntFileContent = serializedData->ss;

    return serializedData;
}

void BitmapFontSerializer::buildSerializedData(
    std::shared_ptr<BitmapFontSerializer::SerializedData> serializedData,
    IAssetsManager* assetsManager)
{
    buildSerializedData_general(serializedData, assetsManager);

    buildSerializedData_chars(serializedData, assetsManager);

    buildSerializedData_kernings(serializedData, assetsManager);
}

void BitmapFontSerializer::buildSerializedData_general(
    std::shared_ptr<BitmapFontSerializer::SerializedData> serializedData,
    IAssetsManager* assetsManager)
{
    //info face="fonts/Roboto-Medium-AaNSS-32" rendering3DSize=32 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=2 padding=0,0,0,0 spacing=0,0
    *serializedData->ss << "info face=\"" << serializedData->font->name << "\" rendering3DSize=" << serializedData->font->size << " bold=0 italic=0 charset=\"\" unicode=1 stretchH=100 smooth=1 aa=2 padding=0,0,0,0 spacing=0,0" << "\r\n";

    //common lineHeight=38 base=30 scaleW=1 scaleH=1 pages=1 packed=0 alphaChnl=0 redChnl=0 greenChnl=0 blueChnl=0 ascent=0 descent=0 lineGap=0
    int baseline = serializedData->font->lineHeight - serializedData->font->baseline;
    *serializedData->ss << "common lineHeight=" << serializedData->font->lineHeight << " base=" << baseline << " scaleW=1 scaleH=1 pages=" << serializedData->font->peekPageTexturesList()->count() << " packed=0 alphaChnl=0 redChnl=0 greenChnl=0 blueChnl=0 ascent=" << serializedData->font->ascent << " descent=" << serializedData->font->descent << " lineGap=" << serializedData->font->lineGap << "" << "\r\n";

    if (serializedData->font->peekPageTexturesList()->count() < 1) {
        // Multiple font texture pages not implemented or missing texture at page index of 0.
        throw LogicException(LOC);
    }

    // Get all texture dependecies assets.
    ArrayList<std::shared_ptr<base::Asset>> dependencyAssets{};
    for (int i = 0; i < serializedData->fontAsset->getSubDependenciesList()->count(); i++) {
        std::shared_ptr<AssetIdentifier> dependencyAssetIdentifier = serializedData->fontAsset->getSubDependenciesList()->getDirect(i);

        if (dependencyAssetIdentifier->type == AssetType::Texture) {
            std::shared_ptr<base::Asset> dependencyAsset = assetsManager->getAsset(*dependencyAssetIdentifier);
            if (dependencyAsset == nullptr) {
                throw LogicException(LOC);
            }

            dependencyAssets.appendDirect(dependencyAsset);
        }
    }

    for (int pageIndex = 0; pageIndex < serializedData->font->peekPageTexturesList()->count(); pageIndex++) {
        Texture* pageTexture = serializedData->font->peekPageTexturesList()->getDirect(pageIndex);
        if (pageTexture == nullptr) {
            throw LogicException(LOC);
        }

        // Find the asset for this page texture.
        std::shared_ptr<base::Asset> pageTextureAsset = FindPageTextureAsset(dependencyAssets, pageTexture);
        if (pageTextureAsset == nullptr) {
            throw LogicException(LOC);
        }

        *serializedData->ss << "page fxId=" << pageIndex << " file=\"" << pageTextureAsset->getIdentifier()->id << "\"" << "\r\n";
    }
}

std::shared_ptr<base::Asset> BitmapFontSerializer::FindPageTextureAsset(ArrayList<std::shared_ptr<base::Asset>>& dependencyAssets, Texture* pageTexture) {
    for (int i = 0; i < dependencyAssets.count(); i++) {
        std::shared_ptr<base::Asset> asset = dependencyAssets.getDirect(i);

        if (asset->getType() != AssetType::Texture) {
            throw LogicException(LOC);
        }

        if (asset->getAsTexture() == pageTexture) {
            return asset;
        }
    }

    return nullptr;
}

void BitmapFontSerializer::buildSerializedData_chars(
    std::shared_ptr<BitmapFontSerializer::SerializedData> serializedData,
    IAssetsManager* assetsManager)
{
    //chars count=97
    int charsCount = serializedData->font->peekGlyphsList()->size();
    *serializedData->ss << "chars count=" << charsCount << "\r\n";

    for(int i=0;i<serializedData->font->peekGlyphsList()->size();i++) {
        base::BitmapFont::Glyph* glyph = serializedData->font->peekGlyphsList()->getDirect(i);

        //char fxId=0    x=432  y=59   width=0    height=0    xoffset=0    yoffset=30   xadvance=0    page=0    chnl=0
        int charUnicodeDecCodepoint = glyph->unicodeCodepointDecimal;
        int char_x = 0;
        int char_y = 0;
        int char_width = 0;
        int char_height = 0;

        if (glyph->textureRegion != nullptr) {
            char_x = glyph->textureRegion->getPixelsX(false, false);
            char_y = glyph->textureRegion->getPixelsY(true, true);
            char_width = glyph->textureRegion->getPixelsWidth(false, false);
            char_height = glyph->textureRegion->getPixelsHeight(true, true);
        }

        int char_xoffset = glyph->offsetX;
        // Convert to top_left coords system.
        int char_yoffset = serializedData->font->lineHeight - (glyph->offsetY + char_height);
        int char_xadvance = glyph->advanceX;
        int pageIndex = glyph->pageIndex;
        // The values should have a padded size of 5 (use space suffixes fr the padding).
        *serializedData->ss << "char fxId=" << Math::intToSuffixSpacePaddedString(charUnicodeDecCodepoint, 4) << " x=" << Math::intToSuffixSpacePaddedString(char_x, 4) << " y=" << Math::intToSuffixSpacePaddedString(char_y, 4) << " width=" << Math::intToSuffixSpacePaddedString(char_width, 4) << " height=" << Math::intToSuffixSpacePaddedString(char_height, 4) << " xoffset=" << Math::intToSuffixSpacePaddedString(char_xoffset, 4) << " yoffset=" << Math::intToSuffixSpacePaddedString(char_yoffset, 4) << " xadvance=" << Math::intToSuffixSpacePaddedString(char_xadvance, 4) << " page=" << Math::intToSuffixSpacePaddedString(pageIndex, 4) << " chnl=0" << "\r\n";
    }
}

void BitmapFontSerializer::buildSerializedData_kernings(
    std::shared_ptr<BitmapFontSerializer::SerializedData> serializedData,
    IAssetsManager* assetsManager)
{
    //kernings count=0
    int kerningsCount = serializedData->font->kerningsCount;

    if(kerningsCount > 0) {
        // Kernings not implemented.

        throw LogicException(LOC);
    }

    *serializedData->ss << "kernings count=" << kerningsCount << "\r\n";
}
