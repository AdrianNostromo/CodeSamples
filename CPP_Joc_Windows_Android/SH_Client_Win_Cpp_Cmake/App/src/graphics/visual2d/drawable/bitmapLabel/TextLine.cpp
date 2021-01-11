#include "TextLine.h"
#include <base/exceptions/LogicException.h>
#include <graphics/visual2d/drawable/BitmapLabel.h>

using namespace base;
using namespace base::bitmapLabel;

TextLine::TextChar::TextChar() {
    //void
}

TextLine::TextChar::~TextChar() {
    //void
}

TextLine::TextLine(int lineIndex)
    : lineIndex(lineIndex)
{
    //void
}

float TextLine::getSolidWidth() {
    float r;
    if (charsList.count() > 0) {
        TextChar& lastChar = charsList.getLast()->data;
        r = lastChar.postCharLineSolidWidth;
    } else {
        r = 0.0f;
    }

    return r;
}

float TextLine::getSuffixSpace() {
    float r;
    if (charsList.count() > 0) {
        TextChar& lastChar = charsList.getLast()->data;
        r = lastChar.postCharSuffixSpace;
    } else {
        r = 0.0f;
    }

    return r;
}

float TextLine::getSolidWidthAndSuffixSpace() {
    float r;
    if (charsList.count() > 0) {
        TextChar& lastChar = charsList.getLast()->data;
        r = lastChar.postCharLineSolidWidth + lastChar.postCharSuffixSpace;
    } else {
        r = 0.0f;
    }

    return r;
}

int TextLine::getRenderableCharactersCount() {
    int r = 0;

    for (ListDL<TextChar>::Entry* entry = charsList.getFirst(); entry != nullptr; entry = entry->next) {
        TextChar& textChar = entry->data;

        if (textChar.glyph->textureRegion != nullptr) {
            r++;
        }
    }

    return r;
}

void TextLine::pushGlyph(BitmapFont::Glyph* glyph, int glyphsInsertCount, graphics::TextureArray*& newTextureArray) {
    for (int i = 0; i < glyphsInsertCount; i++) {
        // Note. Fetch this here or it will get the value from the new unuinitialised char.
        float charOffsetX = getSolidWidthAndSuffixSpace();

        // Use a temp value because the pointer won't be valid after insertion.
        // Also don't use append_emplace because the memory leaks detection doesn't work for that.
        TextChar& textChar = charsList.appendEmplace()->data;

        textChar.glyph = glyph;
        textChar.charOffsetX = charOffsetX;

        textChar.postCharLineSolidWidth = textChar.charOffsetX + glyph->width;
        textChar.postCharSuffixSpace = glyph->advanceX - glyph->width;

        if (glyph->textureRegion != nullptr) {
            if (newTextureArray == nullptr) {
                newTextureArray = glyph->textureRegion->getTexture()->textureArray;
            } else if (glyph->textureRegion->getTexture()->textureArray != newTextureArray) {
                // The texture array may not differ.
                throw LogicException(LOC);
            }
        }
    }
}

void TextLine::popChar(int charsToPopCount) {
    for (int i = 0; i < charsToPopCount; i++) {
        charsList.getLast()->remove();
    }
}

TextLine::~TextLine() {
    //void
}
