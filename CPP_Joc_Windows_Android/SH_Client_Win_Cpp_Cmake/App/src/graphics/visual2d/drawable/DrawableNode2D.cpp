#include "DrawableNode2D.h"
#include <assert.h>
#include <iostream>
#include <base/math/util/BoolUtil.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual2d/drawable/IRoot2D.h>

DrawableNode2D::DrawableNode2D()
	: super()
{
	//void
}

void DrawableNode2D::reservedCreate() {
    createMain();
}

void DrawableNode2D::createMain() {
	//void
}

void DrawableNode2D::invalidateDrawSpecs(int invalidationBitMap) {
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

    // Note : (syncDrawSpecsPre == true) Parent is not notified when a invalidation occurs from a syncDrawSpecsPre function action;
    // There is no parent on root.
	if (this->invalidationBitMap == 0 && !isSyncDrawSpecsPreInProgress && parent != nullptr) {
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

void DrawableNode2D::setX(float x) {
	if (this->x == x) {
		return;
	}

	this->x = x;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode2D::InvalidationFlags::transform
	));
}

void DrawableNode2D::setY(float y) {
	if (this->y == y) {
		return;
	}

	this->y = y;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode2D::InvalidationFlags::transform
	));
}

void DrawableNode2D::setPosition(float x, float y) {
	if (this->x == x && this->y == y) {
		return;
	}

	this->x = x;
	this->y = y;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode2D::InvalidationFlags::transform
	));
}

void DrawableNode2D::setScaleX(float scaleX) {
	if (this->scaleX == scaleX) {
		return;
	}

	this->scaleX = scaleX;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode2D::InvalidationFlags::transform
	));
}

void DrawableNode2D::setScaleY(float scaleY) {
	if (this->scaleY == scaleY) {
		return;
	}

	this->scaleY = scaleY;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode2D::InvalidationFlags::transform
	));
}

void DrawableNode2D::setScale(float scaleX, float scaleY) {
	if (this->scaleX == scaleX && this->scaleY == scaleY) {
		return;
	}

	this->scaleX = scaleX;
	this->scaleY = scaleY;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode2D::InvalidationFlags::transform
	));
}

void DrawableNode2D::setRotationD(float rotationD) {
	if (this->rotationD == rotationD) {
		return;
	}

	this->rotationD = rotationD;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode2D::InvalidationFlags::transform
	));
}

void DrawableNode2D::setTintColor(Color& tintColor) {
	if (this->tintColor.equals(&tintColor)) {
		return;
	}

	this->tintColor.set(tintColor);

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		InvalidationFlags::tintColor
	));
}

void DrawableNode2D::setAlpha(float alpha) {
	if (this->alpha == alpha) {
		return;
	}

	this->alpha = alpha;

	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		DrawableNode2D::InvalidationFlags::alpha
	));
}

bool DrawableNode2D::getIsVisible() {
	if(isVisibleCounter < 1) {
		return false;
	}

	return true;
};

void DrawableNode2D::setIsVisibleIncremental(bool isVisible) {
    if(isVisibleCounter == 1 && isVisible) {
        // Tried to make a visible object more_visible. This should never occur.
        throw LogicException(LOC);
    }

	int new_isVisibleCounter = isVisibleCounter + ((isVisible) ? 1 : -1);

	onIsVisibleChangePre(isVisibleCounter, new_isVisibleCounter);

	bool old_global_isVisible = getGlobal_isVisible();

    this->isVisibleCounter = new_isVisibleCounter;
    if(this->isVisibleCounter == 1) {
    	invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
		    DrawableNode2D::InvalidationFlags::empty
		));
    }

    onIsVisibleChanged();

    if(old_global_isVisible != getGlobal_isVisible()) {
        onGlobal_isVisibleChanged();
    }
}

void DrawableNode2D::setIsVisible(bool isVisible) {
	if(this->isVisiblePseudo == isVisible) {
		return;
	}

	this->isVisiblePseudo = isVisible;
	setIsVisibleIncremental(isVisible);
}

void DrawableNode2D::onIsVisibleChangePre(int old_isVisibleCounter, int new_isVisibleCounter) {
	if (new_isVisibleCounter == 0 && old_isVisibleCounter == 1) {
		// Call this here so that the parent will do a syn when this is no longer redered to update the cache value.
		// The parent is optional.
		invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
			DrawableNode2D::InvalidationFlags::childRenderDataCacheSystem
		));
	}
}

void DrawableNode2D::onIsVisibleChanged() {
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

bool DrawableNode2D::getGlobal_isVisible() {
    if(parentGlobal_isVisible && getIsVisible()) {
        return true;
    }

    return false;
}

void DrawableNode2D::setParentGlobal_isVisible(bool parentGlobal_isVisible) {
    if(this->parentGlobal_isVisible == parentGlobal_isVisible) {
        return ;
    }

    bool old_global_isVisible = getGlobal_isVisible();

    this->parentGlobal_isVisible = parentGlobal_isVisible;

    if(old_global_isVisible != getGlobal_isVisible()) {
        onGlobal_isVisibleChanged();
    }
}

void DrawableNode2D::onGlobal_isVisibleChanged() {
	if (!getGlobal_isVisible() && inStage_renderOrderDrawablesListEntry != nullptr) {
		root->removeRenderOrderDrawable(this);
	}
}

void DrawableNode2D::setParent(IContainer2D* parent) {
	if (this->parent == parent) {
		return;
	}

	if (this->parent != nullptr && parent != nullptr) {
		// Not allowed to set from one parent to another. Must set a nullptr parent before the new one is set.
		throw LogicException(LOC);
	}

	onParentChangePre();

	this->parent = parent;

	onParentChanged();
}

ListDL<IDrawableNode2D*>::Entry*& DrawableNode2D::getInParent_drawablesListEntryRef() {
	return inParent_drawablesListEntry;
}

ListDL<IDrawableNode2D*>::Entry*& DrawableNode2D::getInParent_drawablesWithRenderDataListEntryRef() {
	return inParent_drawablesWithRenderDataListEntry;
}

ListDL<IDrawableNode2D*>::Entry*& DrawableNode2D::getInParent_drawablesWithInputInterractionListEntryRef() {
	return inParent_drawablesWithInputInterractionListEntry;
}

void DrawableNode2D::onParentChangePre() {
	if (this->parent != nullptr) {
		// Parent will change and the old parent need to cache the corrent render data again.
		invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
			DrawableNode2D::InvalidationFlags::childRenderDataCacheSystem
		));
	}
}

void DrawableNode2D::onParentChanged() {
	if (this->parent != nullptr) {
		// Invalidate transform because the parent transform may not be the same as the old one (eg: different pos).
		invalidateDrawSpecs(BoolUtil::setBitAtIndexDirect(0,
			DrawableNode2D::InvalidationFlags::transform
		));
	}
}

void DrawableNode2D::setRoot(IRoot2D* root) {
    if (this->root == root) {
        return ;
    }

    onRootChanged_pre();

    this->root = root;

    onRootChanged();
}

void DrawableNode2D::onRootChanged_pre() {
	if (root != nullptr && inStage_renderOrderDrawablesListEntry != nullptr) {
		root->removeRenderOrderDrawable(this);
	}
}

void DrawableNode2D::onRootChanged() {
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

bool DrawableNode2D::isSyncDrawSpecsRequired() {
	if (invalidationBitMap != 0) {
		return true;
	}

	return false;
}

int DrawableNode2D::syncDrawSpecsWhereNeeded(const int parentInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D** preRenderData) {
	if (parentInvalidationBitMap != 0 || invalidationBitMap != 0) {
	    int combinedInvalidationBitMap = BoolUtil::combine(
			invalidationBitMap,
			parentInvalidationBitMap
		);

		// No need to check the global visible because the parents that are not visible will not call this.
        if (root && getIsVisible()) {
            // syncDrawSpecsPre can cause invalidation dispatches.
            isSyncDrawSpecsPreInProgress = true;
            if (syncDrawSpecsPre(combinedInvalidationBitMap, renderer, *preRenderData) != 0) {
                isSyncDrawSpecsPreInProgress = false;
                return -1;
            }
            isSyncDrawSpecsPreInProgress = false;
            combinedInvalidationBitMap = BoolUtil::combine(
                invalidationBitMap,
                parentInvalidationBitMap
            );

            isSyncDrawSpecsInProgress = true;
			if (syncDrawSpecs(combinedInvalidationBitMap, renderer, *preRenderData) != 0) {
                isSyncDrawSpecsInProgress = false;
				return -1;
			}
            isSyncDrawSpecsInProgress = false;
		}else {
			inactiveInvalidationBitMap = BoolUtil::combine(
				inactiveInvalidationBitMap,
				combinedInvalidationBitMap
			);
		}

		invalidationBitMap = 0;
	}

	return 0;
}

int DrawableNode2D::syncDrawSpecsPre(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
	if (inStage_renderOrderDrawablesListEntry == nullptr && getRoot() != nullptr && getGlobal_isVisible()) {
		root->insertRenderOrderDrawable(this);
	}

	return 0;
}

int DrawableNode2D::syncDrawSpecs(int combinedInvalidationBitMap, graphics::IRenderer& renderer, RenderData2D* preRenderData) {
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

	if (BoolUtil::isBitAtIndex(combinedInvalidationBitMap, InvalidationFlags::tintColor)) {
		if (parent != nullptr) {
			globalTintColor.set(parent->getGlobalTintColor());
			globalTintColor.mul(tintColor);
		}else {
			globalTintColor.set(tintColor);
		}
	}

	return 0;
}

int DrawableNode2D::update_transform() {
	// Update local transform.
	if(isLocalTransformDirty) {
		isLocalTransformDirty = false;

		localTransform.idt();

		if (x != 0 || y != 0) {
			localTransform.translate(
				x,
				y
			);
		}

		if (scaleX != 1 || scaleY != 1) {
			localTransform.scale(
				scaleX,
				scaleY
			);
		}

		if (rotationD != 0) {
			localTransform.rotate(rotationD);
		}
	}

	// Update global transform.
	isGlobalTransformInit = true;

	if (parent != nullptr) {
		globalTransform.set(parent->getGlobalTransform(false));
	}
	else {
		globalTransform.idt();
	}
	globalTransform.mul(localTransform);

	return 0;
}

void DrawableNode2D::worldPosToLocalPos(Vector2& screenPos, Vector2& oLocalPos) {
    t1.idt();
    t1.translate(screenPos.x, screenPos.y);

    t2.set(getGlobalTransform(false));
    t2.inv();
    t1.mul(t2);

    t1.getTranslation(oLocalPos);
}

ListDL<IRenderOrderDrawableNode2D*>::Entry*& DrawableNode2D::getInStage_renderOrderDrawablesListEntryRef() {
	return inStage_renderOrderDrawablesListEntry;
}

IRenderOrderDrawableNode2D*& DrawableNode2D::getPreviousDrawableWithRenderableRef() {
	return previousDrawableWithRenderable;
}

IRenderOrderDrawableNode2D* DrawableNode2D::computePreviousOrParentDrawableInStageRenderOrder() {
	if (static_cast<IDrawableNode2D*>(this) == root) {
		// This is the stage, there is no parent or sibling.
		return nullptr;
	}

	// Try to get the previous sibling if any.
	if (inParent_drawablesListEntry == nullptr) {
		// There is no parent, this can occur only for the stage/root.
		throw LogicException(LOC);
	}

	for (ListDL<IDrawableNode2D*>::Entry* siblingEntry = inParent_drawablesListEntry->prev; siblingEntry != nullptr; siblingEntry = siblingEntry->prev) {
		IRenderOrderDrawableNode2D* siblingDrawable = siblingEntry->data;

		// Do this check because previous siblings are not in the list if they are not visible.
		if (siblingDrawable->getInStage_renderOrderDrawablesListEntryRef() != nullptr) {
			return siblingDrawable;
		}
	}
	
	return getParent();;
}

bool DrawableNode2D::getHasRenderable() {
	// This gets fully extended b SizedDrawable2D that returns true.
	return false;
}

graphics::RenderableLiteInstanceVI* DrawableNode2D::getRenderData() {
	// This gets fully extended b SizedDrawable2D that returns true.
	return nullptr;
}

Affine2& DrawableNode2D::getGlobalTransform(bool updateTransformStackIfDirty) {
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
};

bool DrawableNode2D::DoRecursiveInverseOptionalTransformSoftUpdate(DrawableNode2D* currentLayerDrawable) {
	// Go up until the root or first valid_globalTransformPreupdateIndex is reached.
	// Go to parent if exists until the top layer (stage) is reached.
	bool doTransformValidation = false;
	if(currentLayerDrawable->getParent() != nullptr) {
		// Returns true if update_transform layer found and update started.
		//Don't go up if the parent has a precomputed transform already. parent.globalTransformPreupdateIndex==root.globalTransformPreupdateIndexCounter;

		if(currentLayerDrawable->getParent()->castAs<DrawableNode2D*>()->globalTransformPreupdateIndex != currentLayerDrawable->getRoot()->getGlobalTransformPreupdateIndexCounter()) {
			doTransformValidation = DoRecursiveInverseOptionalTransformSoftUpdate(currentLayerDrawable->getParent()->castAs<DrawableNode2D*>());
		}
	}else {
		// Top layer found. currentLayerDrawable must be the root.
		if(static_cast<IDrawableNode2D*>(currentLayerDrawable) != currentLayerDrawable->getRoot()) {
			throw LogicException(LOC);
		}

		// No need to increment this IRoot2D globalTransformPreupdateIndexCounter here.
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

bool DrawableNode2D::getIsGlobalTransformInit() {
	return isGlobalTransformInit;
}

std::shared_ptr<base::IWrappedValue> DrawableNode2D::getPropertyValue(std::string property) {
	if (property == "x") {
        return base::IWrappedValue::new_float(x);
	} else if (property == "y") {
        return base::IWrappedValue::new_float(y);
	} else if (property == "scaleX") {
        return base::IWrappedValue::new_float(scaleX);
	} else if (property == "scaleY") {
        return base::IWrappedValue::new_float(scaleY);
	} else if (property == "rotationD") {
        return base::IWrappedValue::new_float(rotationD);
	} else if (property == "alpha") {
        return base::IWrappedValue::new_float(alpha);
	} else if (property == "visible" || property == "isVisible") {
        return base::IWrappedValue::new_bool(getIsVisible());
	} else if (property == "tintColor") {
        return base::IWrappedValue::new_Color(tintColor);
	} else {
        throw LogicException(LOC);
	}
}

void DrawableNode2D::setPropertyValue(std::string property, std::shared_ptr<base::IWrappedValue> value) {
	if (property == "x") {
        setX(value->getDirectAs_float());
	} else if (property == "y") {
		setY(value->getDirectAs_float());
	} else if (property == "scaleX") {
		setScaleX(value->getDirectAs_float());
	} else if (property == "scaleY") {
		setScaleY(value->getDirectAs_float());
	} else if (property == "rotationD") {
		setRotationD(value->getDirectAs_float());
	} else if (property == "alpha") {
		setAlpha(value->getDirectAs_float());
	} else if (property == "_visibleFloat") {
		if ((value->getDirectAs_float()) == 0) {
			setIsVisible(false);
		} else {
			setIsVisible(true);
		}
	} else if (property == "visible") {
        setIsVisible(value->getDirectAs_bool());
    } else if (property == "tintColor") {
        setTintColor(value->getReferenceAs_Color());
    } else {
        throw LogicException(LOC);
    }
}

bool DrawableNode2D::isInit_StaticAnimatingPropertyStatus(std::string& property) {
    if(staticAnimatingPropertiesMap.getDirect(property, -1) != -1) {
        return true;
    }

    return false;
}

bool DrawableNode2D::isTrue_StaticAnimatingPropertyStatus(std::string& property) {
    int v = staticAnimatingPropertiesMap.getDirect(property, -1);

    if(v == 1) {
        return true;
    }else if(v == 0) {
        return false;
    }

    throw LogicException(LOC);
}

void DrawableNode2D::putStaticAnimatingPropertyStatus(std::string& property, int status) {
    staticAnimatingPropertiesMap.putDirect(property, status);
}

void DrawableNode2D::disposeMain() {
	if(getParent() != nullptr) {
		getParent()->removeChild(this);
	}

	super::disposeMain();
}

DrawableNode2D::~DrawableNode2D() {
    //void
}
