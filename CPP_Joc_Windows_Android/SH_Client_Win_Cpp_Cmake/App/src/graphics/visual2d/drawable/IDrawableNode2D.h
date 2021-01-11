#pragma once

#include <base/gh.h>
#include <graphics/visual/Color.h>
#include <base/math/Vector2.h>
#include <base/math/Affine2.h>
#include <graphics/visual/drawable/IGenericDrawable.h>
#include <graphics/visual2d/drawable/IDynamicPropertiesObject2D.h>
#include <base/object/IObject.h>
#include "IRenderOrderDrawableNode2D.h"
#include <base/list/ListDL.h>

class IContainer2D;
class RenderData2D;
class IRoot2D;
namespace graphics {
	class IRenderer;
};

class IDrawableNode2D : virtual public base::IObject, virtual public IDynamicPropertiesObject2D, virtual public IGenericDrawable, virtual public IRenderOrderDrawableNode2D {
	pub virtual bool getIsVisible() = 0;
	pub virtual void setIsVisibleIncremental(bool isVisible) = 0;
	pub virtual void setIsVisible(bool isVisible) = 0;
	pub virtual void setParentGlobal_isVisible(bool parentGlobal_isVisible) = 0;

	pub virtual IContainer2D* getParent() = 0;
	pub virtual void setParent(IContainer2D* parent) = 0;
	pub virtual ListDL<IDrawableNode2D*>::Entry*& getInParent_drawablesListEntryRef() = 0;
	pub virtual ListDL<IDrawableNode2D*>::Entry*& getInParent_drawablesWithRenderDataListEntryRef() = 0;
	pub virtual ListDL<IDrawableNode2D*>::Entry*& getInParent_drawablesWithInputInterractionListEntryRef() = 0;
	pub virtual IDrawableNode2D* getDisplayTreeMinLeafDrawableWithRenderData() = 0;
	pub virtual IDrawableNode2D* getDisplayTreeMaxLeafDrawableWithRenderData() = 0;

	pub virtual IRoot2D* getRoot() = 0;
	pub virtual void setRoot(IRoot2D* root) = 0;

	pub virtual void setPosition(float x, float y) = 0;
	pub virtual float getX() = 0;
	pub virtual void setX(float x) = 0;
	pub virtual float getY() = 0;
	pub virtual void setY(float y) = 0;

	pub virtual float getScaleX() = 0;
	pub virtual void setScaleX(float scaleX) = 0;

	pub virtual float getScaleY() = 0;
	pub virtual void setScaleY(float scaleY) = 0;

	pub virtual void setScale(float scaleX, float scaleY) = 0;

	pub virtual float getRotationD() = 0;
	pub virtual void setRotationD(float rotationD) = 0;

	pub virtual float getAlpha() = 0;
	pub virtual void setAlpha(float alpha) = 0;

	pub virtual Color* getTintColor() = 0;
	pub virtual void setTintColor(Color& tintColor) = 0;

	pub virtual Affine2& getGlobalTransform(bool updateTransformStackIfDirty) = 0;
	pub virtual bool getIsGlobalTransformInit() = 0;
	pub virtual void worldPosToLocalPos(Vector2& screenPos, Vector2& oLocalPos) = 0;

	pub virtual float getGlobalAlpha() = 0;

	pub virtual Color& getGlobalTintColor() = 0;

	pub virtual void invalidateDrawSpecs(const int invalidationBitMap) = 0;
	pub virtual int syncDrawSpecsWhereNeeded(
		const int parentInvalidationBitMap,
		graphics::IRenderer& renderer,
		RenderData2D** preRenderData
	) = 0;
	pub virtual bool isSyncDrawSpecsRequired() = 0;

	pub ~IDrawableNode2D() override = default;
};
