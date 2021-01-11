#include "TextureRegion.h"
#include <base/math/Math.h>
#include <base/opengl/IGL.h>

TextureRegion::TextureRegion(
    Texture* texture,
    int pixelsX, int pixelsY, int pixelsWidth, int pixelsHeight,
    bool flipTextureY, bool flipRegionY)
    : texture(texture)
{
    setPixelsRegionXYWidthHeight(pixelsX, pixelsY, pixelsWidth, pixelsHeight, true);

    if (flipTextureY) {
        flipUnderlyingTexture(false, flipTextureY);
    }
    if (flipRegionY) {
        flipRegion(false, flipRegionY);
    }
}

/** @param width The width of the texture region. May be negative to flip the sprite when drawn.
* @param height The height of the texture region. May be negative to flip the sprite when drawn. */
void TextureRegion::setPixelsRegionXYWidthHeight(
    int pixelsX, int pixelsY, int pixelsWidth, int pixelsHeight, 
    bool computeUVValues)
{
    this->pixelsX = pixelsX;
    this->pixelsY = pixelsY;
    this->pixelsWidth = pixelsWidth;
    this->pixelsHeight = pixelsHeight;

    if (computeUVValues) {
        float normalisedTexWidth = 1.0f / texture->getWidth();
        float normalisedTexHeight = 1.0f / texture->getHeight();
        setRegionUVU2V2(
            this->pixelsX * normalisedTexWidth,
            this->pixelsY * normalisedTexHeight,
            (this->pixelsX + this->pixelsWidth) * normalisedTexWidth,
            (this->pixelsY + this->pixelsHeight) * normalisedTexHeight,
            false
        );
    }
}

void TextureRegion::setRegionUVU2V2(
    float u, float v, float u2, float v2, 
    bool computePixelValue/* = true*/)
{
	int texWidth = texture->getWidth();
	int texHeight = texture->getHeight();

    if(computePixelValue) {
        // Compute the values before u,v,u2,v2 are possibly adjusted.
        setPixelsRegionXYWidthHeight(
            (int)Math::round(u * texWidth), (int)Math::round(v * texHeight),
            (int)Math::round((u2 - u) * texWidth), (int)Math::round((v2 - v) * texHeight),
            false
        );
    }

	int regionWidth = (int)Math::round(Math::abs(u2 - u) * texWidth);
    int regionHeight = (int)Math::round(Math::abs(v2 - v) * texHeight);

	if (regionWidth == 1 && regionHeight == 1) {
        // For a 1x1 region, adjust UVs toward pixel center to avoid filtering artifacts on AMD GPUs when drawing very stretched.
        // Not sure if this works if u,u2 or v,v2 are inverted. Currently texture regions are not rotated or flipped so this should not be a problem.

        float adjustX = 0.25f / texWidth;
        if(u < u2) {
            u += adjustX;
            u2 -= adjustX;
        }else if(u > u2) {
            u -= adjustX;
            u2 += adjustX;
        }
		float adjustY = 0.25f / texHeight;
        if(v < v2) {
            v += adjustY;
            v2 -= adjustY;
        }else if(v > v2) {
            v -= adjustY;
            v2 += adjustY;
        }
	}

	this->u = u;
	this->v = v;
	this->u2 = u2;
	this->v2 = v2;
}

float TextureRegion::getU() const {
    return u;
};

float TextureRegion::getV() const {
    return v;
};

float TextureRegion::getU2() const {
    return u2;
};

float TextureRegion::getV2() const {
    return v2;
};

int TextureRegion::getPixelsX(bool targetIsFlipTextureX, bool targetIsFlipRegionX) const {
    int val = pixelsX;

    if(targetIsFlipRegionX) {
        val += this->pixelsWidth;
    }
    if(targetIsFlipTextureX) {
        val = texture->getWidth() - val;
    }

    return val;
};

int TextureRegion::getPixelsX2(bool targetIsFlipTextureX, bool targetIsFlipRegionX) const {
    // The "-1" is important to avoid fetching pixels outside of the range.
    int val = pixelsX + pixelsWidth;

    if(targetIsFlipRegionX) {
        val -= this->pixelsWidth;
    }
    if(targetIsFlipTextureX) {
        val = texture->getWidth() - val;
    }

    return val;
};

int TextureRegion::getPixelsY(bool targetIsFlipTextureY, bool targetIsFlipRegionY) const {
    int val = pixelsY;

    if(targetIsFlipRegionY) {
        val += this->pixelsHeight;
    }
    if(targetIsFlipTextureY) {
        val = texture->getHeight() - val;
    }

    return val;
};

int TextureRegion::getPixelsY2(bool targetIsFlipTextureY, bool targetIsFlipRegionY) const {
    // The "-1" is important to avoid fetching pixels outside of the range.
    int val = pixelsY + pixelsHeight;

    if(targetIsFlipRegionY) {
        val -= this->pixelsHeight;
    }
    if(targetIsFlipTextureY) {
        val = texture->getHeight() - val;
    }

    return val;
};

int TextureRegion::getPixelsWidth(bool targetIsFlipTextureX, bool targetIsFlipRegionX) const {
    int val = pixelsWidth;

    if((targetIsFlipTextureX && !targetIsFlipRegionX) || (!targetIsFlipTextureX && targetIsFlipRegionX)) {
        val *= -1;
    }

    return val;
};

int TextureRegion::getPixelsHeight(bool targetIsFlipTextureY, bool targetIsFlipRegionY) const {
    int val = pixelsHeight;

    if((targetIsFlipTextureY && !targetIsFlipRegionY) || (!targetIsFlipTextureY && targetIsFlipRegionY)) {
        val *= -1;
    }

    return val;
};

void TextureRegion::flipRegion(bool flipX, bool flipY) {
	if (flipX) {
        float temp = u;
		u = u2;
		u2 = temp;

        this->pixelsX += this->pixelsWidth;
        this->pixelsWidth *= -1;
	}
	if (flipY) {
		float temp = v;
		v = v2;
		v2 = temp;

        this->pixelsY += this->pixelsHeight;
        this->pixelsHeight *= -1;
	}
}

void TextureRegion::flipUnderlyingTexture(bool flipX, bool flipY) {
    if(flipX) {
        u = 1.0f - u;
        u2 = 1.0f - u2;

        this->pixelsX = texture->getWidth() - this->pixelsX;
        this->pixelsWidth *= -1;
    }
    if(flipY) {
        v = 1.0f - v;
        v2 = 1.0f - v2;

        this->pixelsY = texture->getHeight() - this->pixelsY;
        this->pixelsHeight *= -1;
    }
}

void TextureRegion::reservedDisposeMain() {
    disposeMain();
}

void TextureRegion::disposeMain() {
	//void
}

TextureRegion::~TextureRegion() {
	//void
}
