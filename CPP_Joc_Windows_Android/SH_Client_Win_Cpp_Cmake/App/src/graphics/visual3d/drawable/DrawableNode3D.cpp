#include "DrawableNode3D.h"
#include <assert.h>
#include <iostream>
#include <base/math/util/BoolUtil.h>
#include <base/exceptions/LogicException.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual3d/drawable/IRoot3D.h>

int DrawableNode3D::InstancesCount = 0;

DrawableNode3D::DrawableNode3D()
	: super()
{
	InstancesCount++;
}

void DrawableNode3D::invalidateDrawSpecs(int invalidationBitMap) {
	if(isSyncDrawSpecsInProgress) {
		// Error. Invalidation must not occur while a syncDrawSpecs in in progress. Use syncDrawSpecsPre instead.
		throw LogicException(LOC);
	}

	// Cache the invalidation if not active.
	if (!this->root || !getIsVisible()) {
		this->inactiveInvalidationBitMap = BoolUtil::combine(
			this->inactiveInvalidationBitMap,
			invalidationBitMap
		);

		return;
	}

	if (this->invalidationBitMap == 0 && parent != nullptr) {
		// This is the first invalidation request.
		parent->childSyncDrawSpecsRequested();
	}

	if(BoolUtil::isBitAtIndex(invalidationBitMap, InvalidationFlags::transform)) {
		// Transform invalidation requested or re-requested.
		isLocalTransformDirty = true;

		if(globalTransformPreupdateIndex == getRoot()->getGlobalTransformPreupdateIndexCounter()) {
			// The precomputed transforms branch is no longer valid.
			getRoot()->incrementGlobalTransformPreupdateIndexCounter();
		}
	}

	this->invalidationBitMap = BoolUtil::combine(
		this->invalidationBitMap,
		invalidationBitMap
	);
}

void DrawableNode3D::setX(float x) {
	if (this->pos.x == x) {
		return;
	}

	this->pos.x = x;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setY(float y) {
	if (this->pos.y == y) {
		return;
	}

	this->pos.y = y;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setZ(float z) {
	if (this->pos.z == z) {
		return;
	}

	this->pos.z = z;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setPosition(float x, float y, float z) {
	if (this->pos.x == x && this->pos.y == y && this->pos.z == z) {
		return;
	}

	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setPosition(Vector3& pos) {
	setPosition(pos.x, pos.y, pos.z);
}

Vector3* DrawableNode3D::getScale() {
	return &scale;
}

void DrawableNode3D::setScaleX(float scaleX) {
	if (this->scale.x == scaleX) {
		return;
	}

	this->scale.x = scaleX;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setScaleY(float scaleY) {
	if (this->scale.y == scaleY) {
		return;
	}

	this->scale.y = scaleY;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setScaleZ(float scaleZ) {
	if (this->scale.z == scaleZ) {
		return;
	}

	this->scale.z = scaleZ;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setScale(float scaleX, float scaleY, float scaleZ) {
	if (this->scale.x == scaleX && this->scale.y == scaleY && this->scale.z == scaleZ) {
		return;
	}

	this->scale.x = scaleX;
	this->scale.y = scaleY;
	this->scale.z = scaleZ;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setScale(Vector3& scale) {
	setScale(
        scale.x,
        scale.y,
        scale.z
	);
}

void DrawableNode3D::setRotation(Quaternion& rotation) {
	if (this->rotation.equals(rotation)) {
        return;
	}

    this->rotation.set(rotation);

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setRotation(const Vector3& axis, const float degrees) {
	this->rotation.set(axis, degrees);

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setRotEulerZYX(Vector3& axis, float degrees) {
	float yawZ;
	float pitchY;
	float rollX;
	this->rotation.getEulerAnglesZYXDeg(yawZ, pitchY, rollX);

	if (axis.x == 1.0f) {
		rollX = degrees;
	} else if (axis.y == 1.0f) {
		pitchY = degrees;
	} else if (axis.z == 1.0f) {
		yawZ = degrees;
	} else {
		throw LogicException(LOC);
	}
	this->rotation.setEulerAnglesZYXDeg(yawZ, pitchY, rollX);

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::rotate(const Vector3& axis, const float degrees) {
    this->rotation.mul(*quat.set(axis, degrees));

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::transform
	));
}

void DrawableNode3D::setAlpha(float alpha) {
	if (this->alpha == alpha) {
		return;
	}

	this->alpha = alpha;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode3D::InvalidationFlags::alpha
	));
}

bool DrawableNode3D::getIsVisible() {
    if(isVisibleCounter < 1) {
        return false;
    }

    return true;
};

void DrawableNode3D::setIsVisibleIncremental(bool isVisible) {
    if(isVisibleCounter == 1 && isVisible) {
        // Tried to make a visible object more_visible. This should never occur.
        throw LogicException(LOC);
    }

    bool old_global_isVisible = getGlobal_isVisible();

    this->isVisibleCounter += (isVisible) ? 1 : -1;
    if(this->isVisibleCounter == 1) {
    	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		    DrawableNode3D::InvalidationFlags::empty
		));
    }

    onIsVisibleChanged();

    if(old_global_isVisible != getGlobal_isVisible()) {
        onGlobal_isVisibleChanged();
    }
}

void DrawableNode3D::setIsVisible(bool isVisible) {
	if(this->isVisiblePseudo == isVisible) {
		return;
	}

	this->isVisiblePseudo = isVisible;
	setIsVisibleIncremental(isVisible);
}

void DrawableNode3D::onIsVisibleChanged() {
	if(this->isVisibleCounter == 1 || this->isVisibleCounter == 0) {
	    if (this->root && getIsVisible()) {
		    invalidateDrawSpecs(inactiveInvalidationBitMap);
		    inactiveInvalidationBitMap = 0;
	    }else {
		    // Move invalidationBitMap to inactive to make sure the parent is notified on.
		    inactiveInvalidationBitMap = BoolUtil::combine(
			    invalidationBitMap,
			    inactiveInvalidationBitMap
		    );
		    invalidationBitMap = 0;
	    }
    }
}

bool DrawableNode3D::getGlobal_isVisible() {
    if(parentGlobal_isVisible && getIsVisible()) {
        return true;
    }

    return false;
}

void DrawableNode3D::setParentGlobal_isVisible(bool parentGlobal_isVisible) {
    if(this->parentGlobal_isVisible == parentGlobal_isVisible) {
        return ;
    }

    bool old_global_isVisible = getGlobal_isVisible();

    this->parentGlobal_isVisible = parentGlobal_isVisible;

    if(old_global_isVisible != getGlobal_isVisible()) {
        onGlobal_isVisibleChanged();
    }
}

void DrawableNode3D::onGlobal_isVisibleChanged() {
    //void
}

void DrawableNode3D::setParent(IContainer3D* parent) {
	if (this->parent == parent) {
		return;
	}

	if (this->parent != nullptr && parent != nullptr) {
		// Not allowed to set from one parent to another. Must set a nullptr parent before the new one is set.
		throw LogicException(LOC);
	}

	this->parent = parent;

	if (this->parent != nullptr) {
		// Invalidate transform because the parent transform may not be the same as the old one (eg: different pos).
		invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
			DrawableNode3D::InvalidationFlags::transform
		));
	}
}

void DrawableNode3D::setRoot(IRoot3D* root) {
	if (this->root == root) {
	    return ;
	}

    onRootChanged_pre();

	this->root = root;

    onRootChanged();
}

void DrawableNode3D::onRootChanged_pre() {
    //void
}

void DrawableNode3D::onRootChanged() {
    if (this->root && getIsVisible()) {
        invalidateDrawSpecs(inactiveInvalidationBitMap);
        inactiveInvalidationBitMap = 0;
    }else {
	    // Move invalidationBitMap to inactive to make sure the parent is notified on .
	    inactiveInvalidationBitMap = BoolUtil::combine(
		    invalidationBitMap,
		    inactiveInvalidationBitMap
	    );
	    invalidationBitMap = 0;
    }
}

bool DrawableNode3D::isSyncDrawSpecsRequired() {
	if (invalidationBitMap != 0) {
		return true;
	}

	return false;
}

int DrawableNode3D::syncDrawSpecsWhereNeeded(const int parentInvalidationBitMap, graphics::IRenderer& renderer) {
	if (parentInvalidationBitMap != 0 || invalidationBitMap != 0) {
		int combinedInvalidationBitMap = BoolUtil::combine(
			invalidationBitMap,
			parentInvalidationBitMap
		);

		if (root && getIsVisible()) {
			isSyncDrawSpecsInProgress = true;
			if (syncDrawSpecs(combinedInvalidationBitMap, renderer) != 0) {
				isSyncDrawSpecsInProgress = false;
				return -1;
			}
			isSyncDrawSpecsInProgress = false;
		} else {
			inactiveInvalidationBitMap = BoolUtil::combine(
				inactiveInvalidationBitMap,
				combinedInvalidationBitMap
			);
		}

		invalidationBitMap = 0;
	}

	return 0;
}

int DrawableNode3D::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer) {
	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, InvalidationFlags::transform)) {
		if (update_transform() != 0) {
			return -1;
		}
	}

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, InvalidationFlags::alpha)) {
		globalAlpha = alpha;

		if (parent != nullptr) {
			globalAlpha *= parent->getGlobalAlpha();
		}
	}

	return 0;
}

int DrawableNode3D::update_transform() {
	// Update local transform.
	if(isLocalTransformDirty) {
		isLocalTransformDirty = false;

		localTransform.idt();

		if (pos.x != 0 || pos.y != 0 || pos.z != 0) {
			localTransform.translate(
				pos.x,
				pos.y,
				pos.z
			);
		}

		if (scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f) {
			localTransform.scale(
				scale.x,
				scale.y,
				scale.z
			);
		}

		localTransform.rotate(rotation);
	}

	// Update global transform.
	isGlobalTransformInit = true;

	if (parent != nullptr) {
		globalTransform.set(parent->getGlobalTransform(false));
	} else {
		globalTransform.idt();
	}
	globalTransform.mul(localTransform);

	return 0;
}

void DrawableNode3D::onRenderDataHooked() {
	//void
}

IContainer3D* DrawableNode3D::getParent() {
	if(getIsDisposed()) {
		throw LogicException(LOC);
	}

	return parent;
};

Matrix4& DrawableNode3D::getLocalTransform() {
	return localTransform;
}

Matrix4& DrawableNode3D::getGlobalTransform(bool updateTransformStackIfDirty) {
    if(!isGlobalTransformInit || getRoot() == nullptr) {
        throw LogicException(LOC);
    }
    if(updateTransformStackIfDirty) {
    	// Test if update required.
    	if(globalTransformPreupdateIndex != getRoot()->getGlobalTransformPreupdateIndexCounter()) {
		    // Call a recursive function until the stage is reached and then update the transform in return starting from
		    // the first (starting from the stage) invalid transform drawable.
		    DoRecursiveInverseOptionalTransformSoftUpdate(this);
	    }
    }

    return globalTransform;
}

bool DrawableNode3D::DoRecursiveInverseOptionalTransformSoftUpdate(DrawableNode3D* currentLayerDrawable) {
	// Go up until the root or first valid_globalTransformPreupdateIndex is reached.
	// Go to parent if exists until the top layer (stage) is reached.
	bool doTransformValidation = false;
	if(currentLayerDrawable->getParent() != nullptr) {
		// Returns true if update_transform layer found and update started.
		//Don't go up if the parent has a precomputed transform already. parent.globalTransformPreupdateIndex==root.globalTransformPreupdateIndexCounter;

		if(currentLayerDrawable->getParent()->castAs<DrawableNode3D*>()->globalTransformPreupdateIndex != currentLayerDrawable->getRoot()->getGlobalTransformPreupdateIndexCounter()) {
			doTransformValidation = DoRecursiveInverseOptionalTransformSoftUpdate(currentLayerDrawable->getParent()->castAs<DrawableNode3D*>());
		}
	}else {
		// Top layer found. currentLayerDrawable must be the root.
		if(static_cast<IDrawableNode3D*>(currentLayerDrawable) != currentLayerDrawable->getRoot()) {
			throw LogicException(LOC);
		}

		// No need to increment this IRoot3D globalTransformPreupdateIndexCounter here.
	}

	// Set globalTransformPreupdateIndex to the root value.
	currentLayerDrawable->globalTransformPreupdateIndex = currentLayerDrawable->getRoot()->getGlobalTransformPreupdateIndexCounter();

	if (!doTransformValidation && currentLayerDrawable->isLocalTransformDirty) {
		doTransformValidation = true;
	}

	// Find the first invalid_transform layer starting from the stage.
	if(doTransformValidation) {
		currentLayerDrawable->update_transform();
	}

	return doTransformValidation;
}

bool DrawableNode3D::getIsGlobalTransformInit() {
    return isGlobalTransformInit;
}

std::shared_ptr<base::IWrappedValue> DrawableNode3D::getPropertyValue(std::string property) {
    throw LogicException(LOC);
}

void DrawableNode3D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
    throw LogicException(LOC);
}

void DrawableNode3D::disposeMain() {
	if(getParent() != nullptr) {
		getParent()->removeChild(this);
	}

	super::disposeMain();
}

DrawableNode3D::~DrawableNode3D() {
	InstancesCount--;
}
