#include "TextureAtlas.h"

TextureAtlas::TextureAtlas()
    : super()
{
	//void
}

TextureRegion* TextureAtlas::getRegion(std::string& regionId) {
	return regionsMap.getDirect(regionId, nullptr);
}

TextureAtlas::~TextureAtlas() {
	//void
}
