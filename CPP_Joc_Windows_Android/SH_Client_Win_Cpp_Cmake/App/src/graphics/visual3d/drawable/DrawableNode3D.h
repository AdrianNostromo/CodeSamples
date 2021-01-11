#pragma once

#include <base/gh.h>
#include <graphics/visual3d/drawable/IDrawableNode3D.h>
#include <graphics/visual/Color.h>
#include <graphics/renderer/IRenderer.h>
#include <string>
#include <base/map/Map1D.h>
#include "IContainer3D.h"
#include <memory>
#include <base/object/WrappedValue.h>
#include <base/object/Object.h>
#include <base/math/Vector3.h>

class DrawableNode3D : public base::Object, virtual public IDrawableNode3D {priv typedef base::Object super;pub dCtor(DrawableNode3D);
	// Used for dev.
	priv static int InstancesCount;

	priv Vector3 pos{0.0f, 0.0f, 0.0f};
	priv Vector3 scale{1.0f, 1.0f, 1.0f};
	
	priv Quaternion rotation{};

	//asd_3;// remove alpha because it should be controlled only by materials and vertex_attribute_diffuse_color.
	priv float alpha = 1.0f;

	priv IContainer3D* parent = nullptr;
	priv IRoot3D* root = nullptr;

	priv Matrix4 localTransform;
	// This is used because of transform preupdate logic.
	priv bool isLocalTransformDirty = true;

	priv Matrix4 globalTransform;
	// globalTransform can be used only if this is true;
	priv bool isGlobalTransformInit = false;
	// This is used for the transform preupdate logic.
	priv int globalTransformPreupdateIndex = -1;

	priv float globalAlpha = 1.0f;

	// This is a soft visibility value.
	// This just sets isVisibleCounter as needed.
	// Don't check this value.
	priv bool isVisiblePseudo = true;
    // 1: visible;
    // <1: hidden;
    // >1: logic error;
	priv int isVisibleCounter = 1;
	priv bool parentGlobal_isVisible = true;

	priv Map1D<std::string, int> staticAnimatingPropertiesMap;

	priv Quaternion quat{};

	priv bool isSyncDrawSpecsInProgress = false;

	prot int invalidationBitMap = 0;
	// This bit map caches invalidation flags when not visible or no parent.
	// This cache is activated on parent set and visible set.
	prot int inactiveInvalidationBitMap = -1;

	pub class InvalidationFlags {
		// "empty" is used when a update is required bu nothing changes (eg: is_visible set to true);
		pub static const unsigned int empty = 0;

		pub static const unsigned int transform = 1;
		pub static const unsigned int alpha = 2;

		pub static const unsigned int LAST_USED_BIT_INDEX = alpha;
	};

	pub explicit DrawableNode3D();

	pub void invalidateDrawSpecs(int invalidationBitMap) override;

	pub Vector3* getPos() override { return &pos; };
	pub float getX() override { return pos.x; };
	pub void setX(float x) override;
	pub float getY() override { return pos.y; };
	pub void setY(float y) override;
	pub float getZ() override { return pos.z; };
	pub void setZ(float z) override;
	pub void setPosition(float x, float y, float z) override;
	pub void setPosition(Vector3& pos) override;

	pub Vector3* getScale() final;
	pub float getScaleX() override { return scale.x; };
	pub void setScaleX(float scaleX) override;
	pub float getScaleY() override { return scale.y; };
	pub void setScaleY(float scaleY) override;
	pub float getScaleZ() override { return scale.z; };
	pub void setScaleZ(float scaleZ) override;
	pub void setScale(float scaleX, float scaleY, float scaleZ) override;
	pub void setScale(Vector3& scale) override;

	pub Quaternion* getRotation() override { return &rotation; };
	pub void setRotation(Quaternion& rotation) override;
	pub void setRotation(const Vector3& axis, const float degrees) override;
	pub void setRotEulerZYX(Vector3& axis, float degrees) override;
	pub void rotate(const Vector3& axis, const float degrees) override;

	pub float getAlpha() override { return alpha; };
	pub void setAlpha(float alpha) override;

	pub bool getIsVisible() final;
	pub void setIsVisibleIncremental(bool isVisible) final;
	pub void setIsVisible(bool isVisible) final;
	pub virtual void onIsVisibleChanged();

	pub bool getGlobal_isVisible();
	pub void setParentGlobal_isVisible(bool parentGlobal_isVisible) override;
	pub virtual void onGlobal_isVisibleChanged();

	pub IContainer3D* getParent() override;

	pub float getGlobalAlpha() override { return globalAlpha; };

	pub Matrix4& getLocalTransform() final;
	pub Matrix4& getGlobalTransform(bool updateTransformStackIfDirty) override;
	pub bool getIsGlobalTransformInit() override;

	pub std::shared_ptr<base::IWrappedValue> getPropertyValue(std::string property) override;
	pub void setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) override;

	pub IRoot3D* getRoot() override { return root; };
	pub bool isSyncDrawSpecsRequired() override;
	pub int syncDrawSpecsWhereNeeded(const int parentInvalidationBitMap, graphics::IRenderer& renderer) override;

	prot virtual void onRenderDataHooked();

	prot void setParent(IContainer3D* parent) final;

	prot void setRoot(IRoot3D* root) final;
	prot virtual void onRootChanged_pre();
	prot virtual void onRootChanged();

	prot virtual int syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer);

	// In some cases (fire a projectile after an instant rot on the same tick frame) the transform for a certain visual branch must be
	// updated instantly, call this in a controlled way to achieve that. The transform will still be updated again.
	priv int update_transform();

	// Returns true if update_transform layer found and update started.
	priv static bool DoRecursiveInverseOptionalTransformSoftUpdate(DrawableNode3D* currentLayerDrawable);

	prot void disposeMain() override;
	pub ~DrawableNode3D() override;
};
