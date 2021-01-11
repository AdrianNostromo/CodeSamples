#pragma once

#include <base/gh.h>
#include <graphics/renderer/IRenderer.h>
#include <graphics/visual3d/drawable/IDynamicPropertiesObject3D.h>
#include <graphics/visual/Color.h>
#include <base/math/Vector2.h>
#include <base/math/Matrix4.h>
#include <base/math/Quaternion.h>
#include <base/object/IObject.h>

class IContainer3D;
class IRoot3D;

class IDrawableNode3D : virtual public base::IObject, virtual public IDynamicPropertiesObject3D {
	pub virtual bool getIsVisible() = 0;
	pub virtual void setIsVisibleIncremental(bool isVisible) = 0;
	pub virtual void setIsVisible(bool isVisible) = 0;
	pub virtual void setParentGlobal_isVisible(bool parentGlobal_isVisible) = 0;

	pub virtual IContainer3D* getParent() = 0;
	pub virtual void setParent(IContainer3D* parent) = 0;

	pub virtual IRoot3D* getRoot() = 0;
	pub virtual void setRoot(IRoot3D* root) = 0;

	pub virtual Vector3* getPos() = 0;
	pub virtual float getX() = 0;
	pub virtual void setX(float x) = 0;
	pub virtual float getY() = 0;
	pub virtual void setY(float y) = 0;
	pub virtual float getZ() = 0;
	pub virtual void setZ(float z) = 0;
	pub virtual void setPosition(float x, float y, float z) = 0;
	pub virtual void setPosition(Vector3& pos) = 0;

	pub virtual Vector3* getScale() = 0;
	pub virtual float getScaleX() = 0;
	pub virtual void setScaleX(float scaleX) = 0;
	pub virtual float getScaleY() = 0;
	pub virtual void setScaleY(float scaleY) = 0;
	pub virtual float getScaleZ() = 0;
	pub virtual void setScaleZ(float scaleZ) = 0;
	pub virtual void setScale(float scaleX, float scaleY, float scaleZ) = 0;
	pub virtual void setScale(Vector3& scale) = 0;

	pub virtual Quaternion* getRotation() = 0;
	pub virtual void setRotation(Quaternion& rotation) = 0;
	pub virtual void setRotation(const Vector3& axis, const float degrees) = 0;
	pub virtual void setRotEulerZYX(Vector3& axis, float degrees) = 0;
	pub virtual void rotate(const Vector3& axis, const float degrees) = 0;

	pub virtual float getAlpha() = 0;
	pub virtual void setAlpha(float alpha) = 0;

	pub virtual Matrix4& getLocalTransform() = 0;
	pub virtual Matrix4& getGlobalTransform(bool updateTransformStackIfDirty) = 0;
	pub virtual bool getIsGlobalTransformInit() = 0;

	pub virtual float getGlobalAlpha() = 0;

	pub virtual void invalidateDrawSpecs(const int invalidationBitMap) = 0;
	pub virtual int syncDrawSpecsWhereNeeded(
		const int parentInvalidationBitMap,
		graphics::IRenderer& renderer
	) = 0;
	pub virtual bool isSyncDrawSpecsRequired() = 0;
	
	pub ~IDrawableNode3D() override = default;
};
