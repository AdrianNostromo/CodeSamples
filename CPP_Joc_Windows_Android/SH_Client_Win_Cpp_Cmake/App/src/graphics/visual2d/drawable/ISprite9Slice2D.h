#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>

class TextureRegion;
class Slice9SizePercents;
class Bounds2D;

namespace base {
class ISprite9Slice2D : virtual public IInteractiveDrawable2D {
	pub virtual const TextureRegion* getTextureRegion() = 0;
	pub virtual void setTextureRegion(TextureRegion* textureRegion, Slice9SizePercents const* sliceSizePercents = nullptr) = 0;

	pub virtual Slice9SizePercents const& getSliceSizePercents() = 0;
	pub virtual void setSliceSizePercents(Slice9SizePercents const& sliceSizePercents) = 0;

	pub virtual bool getDrawCenterRegion() = 0;
	pub virtual void setDrawCenterRegion(bool drawCenterRegion) = 0;

	pub virtual Bounds2D const& getInnerBounds() = 0;
	pub virtual void setInnerBounds(Bounds2D& innerBounds) = 0;

	pub virtual float getInnerBoundsMinX() = 0;
	pub virtual void setInnerBoundsMinX(float innerBoundsMinX) = 0;
	
	pub virtual float getInnerBoundsMinY() = 0;
	pub virtual void setInnerBoundsMinY(float innerBoundsMinY) = 0;
	
	pub virtual float getInnerBoundsMaxX() = 0;
	pub virtual void setInnerBoundsMaxX(float innerBoundsMaxX) = 0;
	
	pub virtual float getInnerBoundsMaxY() = 0;
	pub virtual void setInnerBoundsMaxY(float innerBoundsMaxY) = 0;

    pub virtual ~ISprite9Slice2D() = default;
};
};
