#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>
#include <string>

namespace base {
class IBitmapLabel : virtual public IInteractiveDrawable2D {
    pub class GlobalPosRounding {
        pub static const int NONE = 0;
        pub static const int ROUND = 1;
        pub static const int FLOOR = 2;
        pub static const int CEIL = 3;

    };

    pub virtual std::string& getFontName() = 0;
    pub virtual void setFontName(std::string& fontName) = 0;
    pub virtual void setFontNameDirect(std::string fontName) = 0;

    pub virtual int getFontSize() = 0;
    pub virtual void setFontSize(int fontSize) = 0;

    pub virtual int getFontWeight() = 0;
    pub virtual void setFontWeight(int fontWeight) = 0;

    pub virtual std::string& getCharactersRange() = 0;
    pub virtual void setCharactersRange(std::string& charactersRange) = 0;
    pub virtual void setCharactersRangeDirect(std::string charactersRange) = 0;

	pub virtual std::string getText() = 0;
    pub virtual void setTextReference(std::string& text) = 0;
    pub virtual void setTextDirect(std::string text) = 0;

    pub virtual float computeLineHeight() = 0;

    pub virtual ~IBitmapLabel() = default;
};
};
