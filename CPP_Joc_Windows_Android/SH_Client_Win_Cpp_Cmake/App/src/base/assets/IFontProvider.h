#pragma once

#include <base/gh.h>
#include <string>

namespace base {
	class BitmapFont;
};

namespace base {
class IFontProvider {
	pub virtual base::BitmapFont* getFont(
		std::string& fontName, int fontSize, int fontWeight, std::string& charactersRange,
		bool retNullOnBadParams, bool mustExits) = 0;
	
	pub virtual ~IFontProvider() = default;
};
};
