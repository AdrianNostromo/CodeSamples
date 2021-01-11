#include "BitmapFontParser.h"
#include <base/util/StringUtil.h>
#include <base/assets/util/AssetType.h>
#include <base/MM.h>

BitmapFontParser::BitmapFontParser(std::shared_ptr<std::string> atlassBuffer, std::string& assetFolderPath, bool flipY)
    : atlassBuffer(atlassBuffer), assetFolderPath(assetFolderPath), flipY(flipY)
{
    //void
}

BitmapFontParser::~BitmapFontParser() {
    //void
}

void BitmapFontParser::parseData() {
    cLineIndex = 0;
    bool checkDataStructure = true;

    prepareData();

    parseInfo(checkDataStructure);
    parseCommon(checkDataStructure);
    parsePagesList(checkDataStructure);

    parseGlyphsCount(checkDataStructure);
    parseGlyphsList(checkDataStructure);

    parsePageKerningsCount();
    if(kerningsCount != 0) {
        // Kernings currently not implemented.
        throw LogicException(LOC);
    }
}

int BitmapFontParser::buildGlyphs(base::BitmapFont* font, ArrayList<std::shared_ptr<AssetLoaderDependency>>* loaderDependenciesList) {
    for (int i = 0; i < glyphsList.size(); i++) {
        Glyph& glyphConfig = glyphsList.getReference(i);

        TextureRegion* texReg = nullptr;
        if (glyphConfig.page >= 0) {
            Texture* texture = getTextureAssetFromDepsWithPageIndex(glyphConfig.page, loaderDependenciesList);
            if (!texture) {
                return -1;
            }

            if (glyphConfig.width > 0 && glyphConfig.height > 0) {
                texReg = new TextureRegion(
                    texture,
                    glyphConfig.x, glyphConfig.y,
                    glyphConfig.width, glyphConfig.height,
                    true, flipY
                );
            }
        }

        base::BitmapFont::Glyph* glyph = new base::BitmapFont::Glyph(
            glyphConfig.fxId,
            glyphConfig.page,
            texReg,
            // Invert "yoffset" to convert from topLeft origin (the font uses) to app bottomLeft origin.
            glyphConfig.xoffset, common.lineHeight - (glyphConfig.height + glyphConfig.yoffset),
            glyphConfig.width, glyphConfig.height,
            glyphConfig.xadvance
        );

        int key = glyphConfig.fxId;
        font->initGlyph(
            key,
            glyph
        );
    }

    return 0;
}

Texture* BitmapFontParser::getTextureAssetFromDepsWithPageIndex(int pageIndex, ArrayList<std::shared_ptr<AssetLoaderDependency>>* loaderDependenciesList) {
    if (pageIndex < 0) {
        // Glyphs that don't have a visual representation have a pageIndex of -1.
        return 0;
    }

    Page& page = pagesList.getReference(pageIndex);
    std::string texName = page.file;

    for (int i = 0; i < loaderDependenciesList->size(); i++) {
        std::shared_ptr<AssetLoaderDependency> dep = loaderDependenciesList->getDirect(i);
        if (dep->assetIdentifier->type == AssetType::Texture && dep->assetIdentifier->id == texName) {
            if (!dep->asset) {
                return nullptr;
            }

            return dep->asset->getAsTexture();
        }
    }

    return nullptr;
}

void BitmapFontParser::prepareData() {
    linesList = StringUtil::splitToLines(*atlassBuffer, true);
}

void BitmapFontParser::parseInfo(bool checkDataStructure) {
    //info face="font_Roboto_Medium_ttf_ffffffff_AaN_32" rendering3DSize=9 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=2 padding=0,0,0,0 spacing=0,0

    if(cLineIndex >= linesList->size()) {
        throw LogicException(LOC);
    }
    std::string& line = linesList->getReference(cLineIndex);
    cLineIndex++;

    std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
    if(partsList->size() != 12) {
        throw LogicException(LOC);
    }

    if(partsList->getReference(0) != "info") {
        throw LogicException(LOC);
    }

    StringUtil::parsePropertyValueS(partsList->getReference(1), "face", "=", &info.face);
    StringUtil::parsePropertyValueI(partsList->getReference(2), "rendering3DSize", "=", &info.rendering3DSize);
    StringUtil::parsePropertyValueI(partsList->getReference(3), "bold", "=", &info.bold);
    StringUtil::parsePropertyValueI(partsList->getReference(4), "italic", "=", &info.italic);
    StringUtil::parsePropertyValueS(partsList->getReference(5), "charset", "=", &info.charset);
    StringUtil::parsePropertyValueI(partsList->getReference(6), "unicode", "=", &info.unicode);
    StringUtil::parsePropertyValueI(partsList->getReference(7), "stretchH", "=", &info.stretchH);
    StringUtil::parsePropertyValueI(partsList->getReference(8), "smooth", "=", &info.smooth);
    StringUtil::parsePropertyValueI(partsList->getReference(9), "aa", "=", &info.aa);
    StringUtil::parsePropertyValueIIII(partsList->getReference(10), "padding", "=", ",", &info.padding[0], &info.padding[1], &info.padding[2], &info.padding[3]);
    StringUtil::parsePropertyValueII(partsList->getReference(11), "spacing", "=", ",", &info.spacing[0], &info.spacing[1]);
}

void BitmapFontParser::parseCommon(bool checkDataStructure) {
    // common lineHeight=11 base=8 scaleW=1 scaleH=1 pages=1 packed=0 alphaChnl=0 redChnl=0 greenChnl=0 blueChnl=0 ascent=0 descent=0 lineGap=0

    if(cLineIndex >= linesList->size()) {
        throw LogicException(LOC);
    }
    std::string& line = linesList->getReference(cLineIndex);
    cLineIndex++;

    std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
    if(partsList->size() != 14) {
        throw LogicException(LOC);
    }

    if(partsList->getReference(0) != "common") {
        throw LogicException(LOC);
    }

    StringUtil::parsePropertyValueI(partsList->getReference(1), "lineHeight", "=", &common.lineHeight);
    StringUtil::parsePropertyValueI(partsList->getReference(2), "base", "=", &common.base);
    StringUtil::parsePropertyValueI(partsList->getReference(3), "scaleW", "=", &common.scaleW);
    StringUtil::parsePropertyValueI(partsList->getReference(4), "scaleH", "=", &common.scaleH);
    StringUtil::parsePropertyValueI(partsList->getReference(5), "pages", "=", &common.pagesCount);
    StringUtil::parsePropertyValueI(partsList->getReference(6), "packed", "=", &common.packed);
    StringUtil::parsePropertyValueI(partsList->getReference(7), "alphaChnl", "=", &common.alphaChnl);
    StringUtil::parsePropertyValueI(partsList->getReference(8), "redChnl", "=", &common.redChnl);
    StringUtil::parsePropertyValueI(partsList->getReference(9), "greenChnl", "=", &common.greenChnl);
    StringUtil::parsePropertyValueI(partsList->getReference(10), "blueChnl", "=", &common.blueChnl);
    StringUtil::parsePropertyValueI(partsList->getReference(11), "ascent", "=", &common.ascent);
    StringUtil::parsePropertyValueI(partsList->getReference(12), "descent", "=", &common.descent);
    StringUtil::parsePropertyValueI(partsList->getReference(13), "lineGap", "=", &common.lineGap);
}

void BitmapFontParser::parsePagesList(bool checkDataStructure) {
    if(common.pagesCount <= 0) {
        throw LogicException(LOC);
    }

    for(int i=0;i<common.pagesCount;i++) {
        parsePage(checkDataStructure);

        checkDataStructure = false;
    }
}

void BitmapFontParser::parsePage(bool checkDataStructure) {
    // page fxId=0 file="font_Roboto_Medium_ttf_ffffffff_AaN_32.png"

    // Process each line until an empty_line or end_of_array is encountered;
    int pageIndex = pagesList.size();
    Page* page = pagesList.append_emplace();

    if(cLineIndex >= linesList->size()) {
        throw LogicException(LOC);
    }
    std::string& line = linesList->getReference(cLineIndex);
    cLineIndex++;

    std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
    if(partsList->size() != 3) {
        throw LogicException(LOC);
    }

    if(partsList->getReference(0) != "page") {
        throw LogicException(LOC);
    }

    StringUtil::parsePropertyValueI(partsList->getReference(1), "fxId", "=", &page->fxId);
    if(page->fxId != pageIndex) {
        // The pages must be in order and start from 0.
        throw LogicException(LOC);
    }

    std::string s;
    StringUtil::parsePropertyValueS(partsList->getReference(2), "file", "=", &s);
    StringUtil::removeCharacter(s, '"');
    page->file = assetFolderPath + s;
}

void BitmapFontParser::parseGlyphsCount(bool checkDataStructure) {
    // chars count=97

    if(cLineIndex >= linesList->size()) {
        throw LogicException(LOC);
    }
    std::string& line = linesList->getReference(cLineIndex);
    cLineIndex++;

    std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
    if(partsList->size() != 2) {
        throw LogicException(LOC);
    }

    if(partsList->getReference(0) != "chars") {
        throw LogicException(LOC);
    }

    StringUtil::parsePropertyValueI(partsList->getReference(1), "count", "=", &glyphsCount);
}

void BitmapFontParser::parseGlyphsList(bool checkDataStructure) {
    if(glyphsCount <= 0) {
        throw LogicException(LOC);
    }

    for(int i=0;i<glyphsCount;i++) {
        parseGlyph(checkDataStructure);

        checkDataStructure = false;
    }
}

void BitmapFontParser::parseGlyph(bool checkDataStructure) {
    // char fxId=100  x=92   y=21   width=5    height=7    xoffset=0    yoffset=1    xadvance=6    page=0    chnl=0

    Glyph* glyph = glyphsList.append_emplace();

    if(cLineIndex >= linesList->size()) {
        throw LogicException(LOC);
    }
    std::string& line = linesList->getReference(cLineIndex);
    cLineIndex++;

    std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
    if(partsList->size() != 11) {
        throw LogicException(LOC);
    }

    if(partsList->getReference(0) != "char") {
        throw LogicException(LOC);
    }

    StringUtil::parsePropertyValueI(partsList->getReference(1), "fxId", "=", &glyph->fxId);

    StringUtil::parsePropertyValueI(partsList->getReference(2), "x", "=", &glyph->x);
    StringUtil::parsePropertyValueI(partsList->getReference(3), "y", "=", &glyph->y);

    StringUtil::parsePropertyValueI(partsList->getReference(4), "width", "=", &glyph->width);
    StringUtil::parsePropertyValueI(partsList->getReference(5), "height", "=", &glyph->height);

    StringUtil::parsePropertyValueI(partsList->getReference(6), "xoffset", "=", &glyph->xoffset);
    StringUtil::parsePropertyValueI(partsList->getReference(7), "yoffset", "=", &glyph->yoffset);

    StringUtil::parsePropertyValueI(partsList->getReference(8), "xadvance", "=", &glyph->xadvance);

    StringUtil::parsePropertyValueI(partsList->getReference(9), "page", "=", &glyph->page);

    StringUtil::parsePropertyValueI(partsList->getReference(10), "chnl", "=", &glyph->chnl);
}

void BitmapFontParser::parsePageKerningsCount() {
    // kernings count=0

    if(cLineIndex >= linesList->size()) {
        throw LogicException(LOC);
    }
    std::string& line = linesList->getReference(cLineIndex);
    cLineIndex++;

    std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, " ", false);
    if(partsList->size() != 2) {
        throw LogicException(LOC);
    }

    if(partsList->getReference(0) != "kernings") {
        throw LogicException(LOC);
    }

    StringUtil::parsePropertyValueI(partsList->getReference(1), "count", "=", &kerningsCount);
}
