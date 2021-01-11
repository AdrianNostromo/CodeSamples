#include "BitmapFont.h"

using namespace base;

BitmapFont::Glyph::Glyph(
    int unicodeCodepointDecimal,
    int pageIndex,
    TextureRegion* textureRegion,
    int offsetX, int offsetY,
    int width, int height,
    int advanceX
)
    : unicodeCodepointDecimal(unicodeCodepointDecimal),
      pageIndex(pageIndex),
      textureRegion(textureRegion),
      offsetX(offsetX), offsetY(offsetY),
      width(width), height(height),
      advanceX(advanceX)
{
    //void
}

BitmapFont::Glyph::~Glyph() {
    if(textureRegion != nullptr) {
        delete textureRegion;

        textureRegion = nullptr;
    }
}

BitmapFont::BitmapFont(
    std::string& name,
    int size,
    int lineHeight, int baseline,
    int ascent, int descent, int lineGap,
    int kerningsCount)
    : super(),
    name(name),
    size(size),
    lineHeight(lineHeight), baseline(baseline),
    ascent(ascent), descent(descent), lineGap(lineGap),
    kerningsCount(kerningsCount)
{
    //void
}

BitmapFont::Glyph* BitmapFont::getGlyph(int keyCode) {
    return glyphsMap.getDirect(keyCode, nullptr);
}

void BitmapFont::initGlyph(int keyCode, Glyph* glyph) {
    if(keyCode < 0) {
        throw LogicException(LOC);
    }

    if(glyphsMap.containsKey(keyCode)) {
        throw LogicException(LOC);
    }

    glyphsMap.putDirect(keyCode, glyph);
    
    // Insert so the glyphs are sorted. This is useful when the font is saved to a file.
    insertGlyphToListSorted(glyph);
}

int BitmapFont::getOrCreatePageIndex(Texture* pageTexture) {
    int pageIndex = pageTexturesList.indexOfDirect(pageTexture);
    if (pageIndex < 0) {
        pageIndex = pageTexturesList.count();

        pageTexturesList.insertDirect(pageIndex, pageTexture);
    }

    return pageIndex;
}

ArrayList<Texture*>* BitmapFont::peekPageTexturesList() {
    return &pageTexturesList;
}

ArrayList<BitmapFont::Glyph*>* BitmapFont::peekGlyphsList() {
    return &glyphsList;
}

void BitmapFont::insertGlyphToListSorted(Glyph* glyph) {
    int insertIndex = glyphsList.size();
    while(insertIndex > 0 && glyph->unicodeCodepointDecimal < glyphsList.getDirect(insertIndex - 1)->unicodeCodepointDecimal) {
        insertIndex--;
    }

    glyphsList.insertDirect(insertIndex, glyph);
}

BitmapFont::~BitmapFont() {
    //void
}
