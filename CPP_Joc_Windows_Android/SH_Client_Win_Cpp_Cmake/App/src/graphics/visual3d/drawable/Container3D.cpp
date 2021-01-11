#include "Container3D.h"
#include <assert.h>
#include <base/math/util/BoolUtil.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual3d/drawable/IRoot3D.h>

Container3D::Container3D()
	:super()
{
	//void
}

void Container3D::childSyncDrawSpecsRequested() {
	isAnyChildSyncDrawSpecsRequired = true;

	if (getParent() != nullptr) {
		getParent()->childSyncDrawSpecsRequested();
	}
}

int Container3D::getChildrensCount() {
	return drawablesList.count();
}

IDrawableNode3D* Container3D::getChildAt(int index) {
	return drawablesList.getDirect(index);
}

void Container3D::addChild(IDrawableNode3D* drawable) {
	addChildAt(drawable, drawablesList.size());
}

void Container3D::addChildAt(IDrawableNode3D* drawable, unsigned int childIndex) {
    if(drawable == this) {
        throw LogicException(LOC);
    }
    if(drawable->getParent()) {
		drawable->getParent()->removeChild(drawable);
    }

	drawablesList.insertReference(childIndex, drawable);

    drawable->setParent(this);
    drawable->setRoot(getRoot());
}

void Container3D::removeChild(IDrawableNode3D* drawable) {
	if(getIsDisposed()) {
		throw LogicException(LOC);
	}

	assert(drawable->getParent() == this);

    drawable->setRoot(nullptr);
	drawable->setParent(nullptr);

	int index = drawablesList.indexOf(drawable);
	assert(index < (long)drawablesList.size());

	drawablesList.remove(index);
}

void Container3D::onGlobal_isVisibleChanged() {
    bool global_isVisible = getGlobal_isVisible();
    for (int i = 0; i< drawablesList.size(); i++) {
        IDrawableNode3D* drawable = *drawablesList.getPointer(i);

        drawable->setParentGlobal_isVisible(global_isVisible);
    }

    super::onGlobal_isVisibleChanged();
}

void Container3D::onRootChanged() {
    super::onRootChanged();

    IRoot3D* root = getRoot();
	for (int i = 0; i< drawablesList.size(); i++) {
		IDrawableNode3D* drawable = *drawablesList.getPointer(i);

		drawable->setRoot(root);
	}
}

int Container3D::syncDrawSpecsWhereNeeded(const int parentInvalidationBitMap, graphics::IRenderer& renderer) {
	// Compute here because invalidationBitMap becomes 0 in the super call.
	int combinedInvalidationBitMap = BoolUtil::combine(
		invalidationBitMap,
		parentInvalidationBitMap
	);

	int r = super::syncDrawSpecsWhereNeeded(parentInvalidationBitMap, renderer);
	if (r != 0) {
		return r;
	}

	if (combinedInvalidationBitMap != 0 || isAnyChildSyncDrawSpecsRequired) {
		if (getIsVisible() && (getParent() || static_cast<IContainer3D*>(this) == getRoot())) {
			for (int i = 0; i < drawablesList.size(); i++) {
				IDrawableNode3D* entry = *drawablesList.getPointer(i);

				if (combinedInvalidationBitMap != 0 || entry->isSyncDrawSpecsRequired()) {
					if (entry->syncDrawSpecsWhereNeeded(
						combinedInvalidationBitMap,
						renderer
					) != 0) {
						return -1;
					}
				}
			}
		}

		isAnyChildSyncDrawSpecsRequired = false;
	}

	return 0;
}

float Container3D::getGlobalAlpha() {
	return super::getGlobalAlpha();
}

bool Container3D::isSyncDrawSpecsRequired() {
	bool b = super::isSyncDrawSpecsRequired();
	if (!b && isAnyChildSyncDrawSpecsRequired) {
		return true;
	}

	return b;
}

void Container3D::disposeMain() {
	// Remove all children.
	for (int i=drawablesList.size()-1; i>=0; i--) {
		IDrawableNode3D *childEntry = *drawablesList.getPointer(i);

		removeChild(childEntry);
	}

	super::disposeMain();
}

Container3D::~Container3D() {
    //void
}
