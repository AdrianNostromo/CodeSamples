#include "V3DM_Drawables.h"
#include <base/visual3d/IManagedDynamicVisual.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <base/visual3d/Visual3DUtil.h>
#include <base/exceptions/LogicException.h>
#include <graphics/visual3d/drawable/IContainer3D.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace worldGame3D;

base::EntityEvent* V3DM_Drawables::EVENT_visual3D_visibleChanged = newt base::EntityEvent(false, "EVENT_visual3D_visibleChanged");
base::EntityEvent* V3DM_Drawables::EVENT_visual3D_updateVisual = newt base::EntityEvent(false, "EVENT_visual3D_updateVisual");

V3DM_Drawables::V3DM_Drawables(
	IWorldEntity* entity,
	Visual3DTemplate* t,
	ArrayList<IDrawableNode3D*>* drawablesList, Map1D<std::string, IDrawableNode3D*>* drawablesMap, IDrawableNode3D* rootDrawable)
	: super(entity, t),
	drawablesList(drawablesList), drawablesMap(drawablesMap), rootDrawable(rootDrawable)
{
	//void
}

void V3DM_Drawables::createBMain() {
	super::createBMain();

	if (!isPositionalSyncListenerInitialised) {
		// The sync listeners were ot initialised;
		throw LogicException(LOC);
	}
}

void V3DM_Drawables::setIsPositionalSyncListenerEnabled(bool isPositionalSyncListenerEnabled) {
	if (!isPositionalSyncListenerInitialised || this->isPositionalSyncListenerEnabled != isPositionalSyncListenerEnabled) {
		this->isPositionalSyncListenerEnabled = isPositionalSyncListenerEnabled;
		isPositionalSyncListenerInitialised = true;

		if (!this->isPositionalSyncListenerEnabled) {
			syncAutoListenersList.clear();
		} else {
			e->addEventListener(
				IWorldEntity::EVENT_syncPos->type, IWorldEntity::EVENT_syncRot->type, IWorldEntity::EVENT_syncCardinalRotation->type,
				std::bind(&V3DM_Drawables::onEvent, this, std::placeholders::_1, std::placeholders::_2),
				syncAutoListenersList
			);
			e->addEventListener(
				IWorldEntity::EVENT_syncScale->type,
				std::bind(&V3DM_Drawables::onEvent, this, std::placeholders::_1, std::placeholders::_2),
				syncAutoListenersList
			);
		}
	}
}

IWorldEntity* V3DM_Drawables::getEntity() {
	return super::getEntity();
}

ArrayList<IDrawableNode3D*>* V3DM_Drawables::peekDrawablesList() {
	return drawablesList;
}

void V3DM_Drawables::createMain() {
	super::createMain();

	for (int i = drawablesList->size() - 1; i >= 0; i--) {
		IDrawableNode3D* drawable = *drawablesList->getPointer(i);
		if (drawable != nullptr) {
			IManagedDynamicVisual* lDrawable = dynamic_cast<IManagedDynamicVisual*>(drawable);
			if (lDrawable != nullptr) {
				lDrawable->reservedCreate();
			}
		}
	}

	requireVisualsUpdate();
}

void V3DM_Drawables::requireVisualsUpdate() {
	if (!isVisualsDirty && e->getIsInGameWorldAndActive()) {
		isVisualsDirty = true;

		entity->getHandler()->addDrawableModuleToVisualUpdate(this);
	}
}

void V3DM_Drawables::requireManagedVisualUpdate(IManagedDynamicVisual* targetManagedVisual) {
	requireVisualsUpdate();
}

IContainer3D* V3DM_Drawables::getVisualsParent() {
	return visualsParent;
}

void V3DM_Drawables::setVisualsParent(IContainer3D* visualsParent) {
	if (this->visualsParent == visualsParent) {
		return;
	}

	if (rootDrawable->getParent() != nullptr) {
		rootDrawable->getParent()->removeChild(rootDrawable);
	}
	this->visualsParent = visualsParent;

	if (this->visualsParent != nullptr && entity->getIsInGameWorldAndActive()) {
		this->visualsParent->addChild(rootDrawable);
	}
}

IContainer3D* V3DM_Drawables::peekRootDrawableAsContainer() {
	return rootDrawable->castAs<IContainer3D*>();
}

Quaternion* V3DM_Drawables::peekRootDrawable_rotation() {
	return rootDrawable->getRotation();
}

Matrix4* V3DM_Drawables::peekRootDrawable_globalTransform(bool updateTransformStackIfDirty) {
	return &rootDrawable->getGlobalTransform(updateTransformStackIfDirty);
}

bool V3DM_Drawables::peekRootDrawable_isGlobalTransformInit() {
	return rootDrawable->getIsGlobalTransformInit();
}

bool V3DM_Drawables::getIsVisible() {
	return rootDrawable->getIsVisible();
}

void V3DM_Drawables::setIsVisibleIncremental(bool visible) {
	rootDrawable->setIsVisibleIncremental(visible);

	e->dispatchEvent(*EVENT_visual3D_visibleChanged);
}

IDrawableNode3D* V3DM_Drawables::getDrawableOptional(std::string id) {
	IDrawableNode3D* v = drawablesMap->getDirect(id);
	if (!v) {
		return nullptr;
	}

	return v;
}

IDrawableNode3D* V3DM_Drawables::getDrawableMustExist(std::string id) {
	IDrawableNode3D* v = drawablesMap->getDirect(id);
	if (!v) {
		throw LogicException(LOC);
	}

	return v;
}

void V3DM_Drawables::connectToGameParts(worldGame3D::IGameWorldGameParts* gwGameParts) {
	super::connectToGameParts(gwGameParts);
	
	if (rootDrawable != nullptr && this->visualsParent != nullptr) {
		this->visualsParent->addChild(rootDrawable);
	}

	if (!isVisualsDirty && e->getIsInGameWorldAndActive()) {
		isVisualsDirty = true;

		entity->getHandler()->addDrawableModuleToVisualUpdate(this);
	}
}

void V3DM_Drawables::disconnectFromGamePartsPre() {
	if (rootDrawable != nullptr && rootDrawable->getParent() != nullptr) {
		rootDrawable->getParent()->removeChild(rootDrawable);
	}

	super::disconnectFromGamePartsPre();
}

void V3DM_Drawables::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IWorldEntity::EVENT_syncPos->type) {
		syncPos(entity->getPos());
	} else if (event.type == IWorldEntity::EVENT_syncRot->type) {
		syncRot(*entity->getRot());
	} else if (event.type == IWorldEntity::EVENT_syncCardinalRotation->type) {
		syncCardinalRotation(entity->getRot()->getAngleAround(Vector3::Z));
	} else if (event.type == IWorldEntity::EVENT_syncScale->type) {
		syncScale(entity->getScale());
	} else {
		throw LogicException(LOC);
	}
}

void V3DM_Drawables::syncPos(Vector3* pos) {
	rootDrawable->setPosition(*pos);
}

void V3DM_Drawables::syncPos(float x, float y, float z) {
	rootDrawable->setPosition(x, y, z);
}

void V3DM_Drawables::syncPosX(float x) {
	rootDrawable->setX(x);
}

void V3DM_Drawables::syncPosY(float y) {
	rootDrawable->setY(y);
}

void V3DM_Drawables::syncPosZ(float z) {
	rootDrawable->setZ(z);
}

void V3DM_Drawables::syncRot(Quaternion& rot) {
	rootDrawable->setRotation(rot);
}

void V3DM_Drawables::syncRot(Vector3& axis, float degrees) {
	rootDrawable->setRotation(axis, degrees);
}

void V3DM_Drawables::syncRotEulerZYX(Vector3& axis, float degrees) {
	rootDrawable->setRotEulerZYX(axis, degrees);
}

void V3DM_Drawables::syncCardinalRotation(float degrees) {
	rootDrawable->setRotEulerZYX(Vector3::Z, degrees);
}

void V3DM_Drawables::syncScale(Vector3* scale) {
	rootDrawable->setScale(*scale);
}

IListEntry*& V3DM_Drawables::getInHandler_VisualUpdateListEntryRef() {
	return inHandler_VisualUpdateListEntryRef;
}

void V3DM_Drawables::updateVisual() {
	isVisualsDirty = false;

	for (int i = drawablesList->size() - 1; i >= 0; i--) {
		IDrawableNode3D* drawable = *drawablesList->getPointer(i);
		if (drawable != nullptr) {
			IManagedDynamicVisual* lDrawable = dynamic_cast<IManagedDynamicVisual*>(drawable);
			if (lDrawable) {
				lDrawable->updateVisual();
			}
		}
	}

	e->dispatchEvent(*EVENT_visual3D_updateVisual);
}

void V3DM_Drawables::disposeMain() {
	syncAutoListenersList.clear();

	if (rootDrawable != nullptr) {
		rootDrawable = nullptr;
	}

	if (drawablesMap != nullptr) {
		delete drawablesMap;
		drawablesMap = nullptr;
	}

	if (drawablesList != nullptr) {
		Visual3DUtil::disposeDrawables(drawablesList);

		delete drawablesList;
		drawablesList = nullptr;
	}

	super::disposeMain();
}

V3DM_Drawables::~V3DM_Drawables() {
	//void
}
