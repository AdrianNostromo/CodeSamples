#pragma once

#include <base/gh.h>
#include <graphics/visual/BitmapFont.h>
#include <base/list/ListDL.h>

namespace base {
    class BitmapLabel;
};

namespace base {
namespace bitmapLabel {
class TextLine {pub dCtor(TextLine);
    pub class TextChar {pub dCtor(TextChar);
        // Char pos from the start of the line.
        pub float charOffsetX = 0.0f;

        pub BitmapFont::Glyph *glyph = nullptr;

        // This ignores the end empty space.
        pub float postCharLineSolidWidth = 0.0f;
        // This is the end empty space.
        pub float postCharSuffixSpace = 0.0f;

        pub explicit TextChar();

        pub ~TextChar();
    };
    
    pub int lineIndex;

    pub float posX = 0.0f;
    pub float posY = 0.0f;

    pub ListDL<TextChar> charsList{LOC};

    pub explicit TextLine(int lineIndex);

    pub float getSolidWidth();

    pub float getSuffixSpace();

    pub float getSolidWidthAndSuffixSpace();

    pub int getRenderableCharactersCount();

    pub void pushGlyph(BitmapFont::Glyph* glyph, int glyphsInsertCount, graphics::TextureArray*& newTextureArray);
    pub void popChar(int charsToPopCount);

    // Note. Non-rendered characters (eg. space) are included at the end of the line even if wider than the wrappingWidth. Wrapping is done only on rendered characters.
    pub void writeLine(
        std::string& charsList, int& cCharIndex,
        BitmapLabel& bitmapLabel, base::BitmapFont* font);

    pub ~TextLine();
};
};
};
