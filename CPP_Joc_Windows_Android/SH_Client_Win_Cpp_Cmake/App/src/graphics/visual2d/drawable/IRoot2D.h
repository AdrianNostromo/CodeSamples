#pragma once

#include <base/gh.h>
#include <graphics/visual2d/drawable/IContainer2D.h>

class IRenderOrderDrawableNode2D;

class IRoot2D : virtual public IContainer2D {
	pub virtual int getGlobalTransformPreupdateIndexCounter() = 0;
	pub virtual void incrementGlobalTransformPreupdateIndexCounter() = 0;

	pub virtual void insertRenderOrderDrawable(IRenderOrderDrawableNode2D* drawable) = 0;
	pub virtual void removeRenderOrderDrawable(IRenderOrderDrawableNode2D* drawable) = 0;

	pub ~IRoot2D() override = default;
};
