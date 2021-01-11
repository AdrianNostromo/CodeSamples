#pragma once

#include <base/gh.h>
#include <graphics/visual/TextureRegion.h>
#include <base/assets/loader/util/AssetLoaderDependency.h>
#include <base/map/Map1D.h>
#include <base/list/ArrayList.h>
#include <base/buffer/Buffer.h>
#include <graphics/visual/BitmapFont.h>

class BitmapFontParser {pub dCtor(BitmapFontParser);
    pub class Info {pub dCtor(Info);
        pub std::string face;
        pub int rendering3DSize;
        pub int bold;
        pub int italic;
        pub std::string charset;
        pub int unicode;
        pub int stretchH;
        pub int smooth;
        pub int aa;
        pub int padding[4];
        pub int spacing[2];

        pub explicit Info() {
            //void
        }
        pub virtual ~Info() {
            //void
        }
    };
    pub class Common {pub dCtor(Common);
        pub int lineHeight;
        pub int base;
        
        pub int scaleW;
        pub int scaleH;
        
        pub int pagesCount;
        pub int packed;
        
        pub int alphaChnl;
        pub int redChnl;
        pub int greenChnl;
        pub int blueChnl;

        pub int ascent;
        pub int descent;
        pub int lineGap;

        pub explicit Common() {
            //void
        }
        pub virtual ~Common() {
            //void
        }
    };
    pub class Page {pub dCtor(Page);
        pub int fxId;
        pub std::string file;

        pub explicit Page() {
            //void
        }
        pub virtual ~Page() {
            //void
        }
    };
    pub class Glyph {pub dCtor(Glyph);
        // Decimal code point (eg: 8230). Also the html code. (eg: &#8230;).
        // Convert to hex to get the regular unicode(eg: U+2026).
        pub int fxId;

        pub int x;
        pub int y;

        pub int width;
        pub int height;

        pub int xoffset;
        pub int yoffset;

        pub int xadvance;

        pub int page;

        int chnl;

        pub explicit Glyph() {
            //void
        }
        pub virtual ~Glyph() {
            //void
        }
    };

    pub Info info{};
    pub Common common{};
    pub ArrayList<Page> pagesList{};
    pub int glyphsCount;
    pub ArrayList<Glyph> glyphsList{};
    pub int kerningsCount;

    pub std::shared_ptr<std::string> atlassBuffer;
    pub std::shared_ptr<ArrayList<std::string>> linesList;
    pub int cLineIndex = 0;

    pub std::string assetFolderPath;
    pub bool flipY = true;

    pub explicit BitmapFontParser(std::shared_ptr<std::string> atlassBuffer, std::string& assetFolderPath, bool flipY);

    pub void parseData();
    pub int buildGlyphs(base::BitmapFont* font, ArrayList<std::shared_ptr<AssetLoaderDependency>>* loaderDependenciesList);
    
    priv void prepareData();

    priv void parseInfo(bool checkDataStructure);
    priv void parseCommon(bool checkDataStructure);

    priv void parsePagesList(bool checkDataStructure);

    priv void parsePage(bool checkDataStructure);
    priv void parseGlyphsCount(bool checkDataStructure);
    priv void parseGlyphsList(bool checkDataStructure);
    priv void parseGlyph(bool checkDataStructure);
    priv void parsePageKerningsCount();

    priv Texture* getTextureAssetFromDepsWithPageIndex(int pageIndex, ArrayList<std::shared_ptr<AssetLoaderDependency>>* loaderDependenciesList);

    pub virtual ~BitmapFontParser();
};
