#pragma once

#include <graphics/visual/TextureRegion.h>
#include <graphics/visual/TextureAtlas.h>
#include <base/map/Map1D.h>
#include <base/object/Object.h>

class TextureAtlas : public base::Object {
private: typedef Object super;
private:
    // Note : Must use "TextureRegion*" instead of "TextureRegion" because the class copy constructor is deleted.
	Map1D<std::string, TextureRegion*> regionsMap{};
public:
	TextureAtlas();

	Map1D<std::string, TextureRegion*>* getRegionsMap() { return &regionsMap; }
	TextureRegion* getRegion(std::string& regionId);

    ~TextureAtlas() override;
};
