#pragma once

#include <base/gh.h>
#include <base/map/Map1D.h>
#include "TextureRegion.h"
#include <base/list/ArrayList.h>
#include <base/object/Object.h>

namespace base {
class BitmapFont : public Object {priv typedef Object super;pub dCtor(BitmapFont);
    pub class Glyph {pub dCtor(Glyph);
        pub int unicodeCodepointDecimal;

        pub int pageIndex;

        // Owned by this.
        pub TextureRegion* textureRegion;

        pub int offsetX;
        // Offset from the bottom of the line (NOT bottom of the baseline).
        pub int offsetY;

        pub int width;
        pub int height;

        pub int advanceX;

        pub explicit Glyph(
            int unicodeCodepointDecimal,
            int pageIndex,
            TextureRegion*textureRegion,
            int offsetX, int offsetY,
            int width, int height,
            int advanceX);

        pub virtual ~Glyph();
    };

    pub std::string name;
    // font_size is the full height of the character from ascender to descender.
    pub int size;

    // These 2 values are extracted from the above ascent, descent and lineGap;
    pub int lineHeight;
    pub int baseline;

    // These are newly implemented and are valid.
    // Note. Bitmap fot parser, serialised got modified to support these variables.
    pub int ascent;
    pub int descent;
    pub int lineGap;

    pub int kerningsCount;

    priv ArrayList<Texture*> pageTexturesList{};

    // Note : Must use "Glyph*" instead of "Glyph" because the class copy constructor is deleted.
    // The key is the decimal unicode code point.
    //asdx;// convert to utf-8 codepoint int(eg: "E2 80 A6" as "14844070"). Do this when utf8 strings are implemented.
    priv Map1D<int, Glyph*> glyphsMap{};
    priv ArrayList<Glyph*> glyphsList{};

    pub explicit BitmapFont(
        std::string& name,
        int size,
        int lineHeight, int baseline,
        int ascent, int descent, int lineGap,
        int kerningsCount);

    pub Glyph* getGlyph(int keyCode);
    pub void initGlyph(int keyCode, Glyph* glyph);

    pub int getOrCreatePageIndex(Texture* pageTexture);
    pub ArrayList<Texture*>* peekPageTexturesList();

    pub ArrayList<Glyph*>* peekGlyphsList();

    priv void insertGlyphToListSorted(Glyph* glyph);

    pub virtual ~BitmapFont();
};
};
