#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>

class TextureRegion;

class ISprite2D : virtual public IInteractiveDrawable2D {
	pub virtual const TextureRegion* getTextureRegion() = 0;
	pub virtual void setTextureRegion(TextureRegion* textureRegion) = 0;

    pub ~ISprite2D() override = default;
};
