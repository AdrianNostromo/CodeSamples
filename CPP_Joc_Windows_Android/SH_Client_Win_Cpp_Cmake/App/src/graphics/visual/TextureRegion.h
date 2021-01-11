#pragma once

#include <base/gh.h>
#include <graphics/visual/Texture.h>

class TextureRegion {pub dCtor(TextureRegion);
	priv Texture* texture = nullptr;

	// Note. These coords are bottom left.

	// left
	priv float u = 0.0f;
	// bottom
	priv float v = 0.0f;
	// right
	priv float u2 = 1.0f;
	// top
	priv float v2 = 1.0f;

	// left
	priv int pixelsX = 0;
	// right
	priv int pixelsY = 0;
    // Note. Can be negative.
    // Width left_to_right
	priv int pixelsWidth = 0;
    // Note. Can be negative.
    // height bottom_to_top
	priv int pixelsHeight = 0;

    pub explicit TextureRegion(
		Texture* texture, 
		int pixelX, int pixelY, int pixelWidth, int pixelHeight,
		bool flipTextureY, bool flipRegionY);

	pub Texture* getTexture() { return texture; };

	pub void setPixelsRegionXYWidthHeight(
		int pixelsX, int pixelsY, int pixelsWidth, int pixelsHeight, 
		bool computeUVValues = true);
	pub void setRegionUVU2V2(
		float u, float v, float u2, float v2, 
		bool computePixelValue = true);

	pub float getU() const;
	pub float getV() const;
	pub float getU2() const;
	pub float getV2() const;

	pub int getPixelsX(bool targetIsFlipTextureX = false, bool targetIsFlipRegionX = false) const;
	pub int getPixelsX2(bool targetIsFlipTextureX = false, bool targetIsFlipRegionX = false) const;
	pub int getPixelsY(bool targetIsFlipTextureY = false, bool targetIsFlipRegionY = false) const;
	pub int getPixelsY2(bool targetIsFlipTextureY = false, bool targetIsFlipRegionY = false) const;
	pub int getPixelsWidth(bool targetIsFlipTextureX = false, bool targetIsFlipRegionX = false) const;
	pub int getPixelsHeight(bool targetIsFlipTextureY = false, bool targetIsFlipRegionY = false) const;

	pub void flipRegion(bool x, bool y);
	pub void flipUnderlyingTexture(bool x, bool y);

	pub void reservedDisposeMain();
	prot virtual void disposeMain();
	pub virtual ~TextureRegion();
};
