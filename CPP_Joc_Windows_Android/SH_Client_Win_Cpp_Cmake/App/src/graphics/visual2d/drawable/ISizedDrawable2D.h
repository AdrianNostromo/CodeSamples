#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/IGeometryDrawable2D.h>
#include <base/geometry/Shape2D.h>
#include <memory>

class ISizedDrawable2D : virtual public IGeometryDrawable2D {
    pub virtual void setSize(float width, float height) = 0;
	pub virtual float getWidth() = 0;
	pub virtual void setWidth(float width) = 0;
	pub virtual float getHeight() = 0;
	pub virtual void setHeight(float height) = 0;

	pub virtual float getAnchorX() = 0;
	pub virtual void setAnchorX(float anchorX) = 0;
	pub virtual float getAnchorY() = 0;
	pub virtual void setAnchorY(float anchorY) = 0;
	pub virtual void setAnchor(float anchorX, float anchorY) = 0;

};
