#include "WE_Base.h"
#include <base/math/util/BoolUtil.h>
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/util/ContentGroup.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <base/component/IComponent_GWWorldEntityComponent.h>

using namespace worldGame3D;

WE_Base::WE_Base(EntityTemplate* t, int localEntityId, std::shared_ptr<ExtraData> extraData)
	: super(IComponent_GWWorldEntityComponent::COMPONENT_CATEGORY),
	t(t), localEntityId(localEntityId), extraData(extraData)
{
	wrapper = std::make_shared<base::WrappedWorldEntity>(this);
	wrapperB = new base::WrappedWorldEntity(this);
}

EntityTemplate* WE_Base::getTemplate() {
    return t;
}

std::shared_ptr<std::string> WE_Base::getInstanceId() {
	return instanceId;
}

void WE_Base::setInstanceId(std::shared_ptr<std::string> instanceId) {
	this->instanceId = instanceId;
}

bool WE_Base::getIsMarkedForRemoval() {
	return isMarkedForRemoval;
}

EntityRemoveReason* WE_Base::getMarkedForRemovalReason() {
	return markedForRemovalReason;
}

void WE_Base::clearIsMarkedForRemoval() {
	isMarkedForRemoval = false;
	markedForRemovalReason = nullptr;
}

void WE_Base::markEntityForRemoval(EntityRemoveReason* removeReason, bool isRequestFromServer, bool disposeAfterRemoval) {
	if (isMarkedForRemoval) {
		throw LogicException(LOC);
	}

	handler->markEntityForRemoval(this, removeReason, isRequestFromServer, disposeAfterRemoval);
}

void WE_Base::fromHandler_setIsMarkedForRemoval(EntityRemoveReason* markedForRemovalReason) {
    if(isMarkedForRemoval) {
        throw LogicException(LOC);
    }

    isMarkedForRemoval = true;
    this->markedForRemovalReason = markedForRemovalReason;

    if(wrapper->entity == nullptr || wrapperB->entity == nullptr) {
	    throw LogicException(LOC);
    }
    wrapper->entity = nullptr;
    wrapperB->entity = nullptr;
}

std::shared_ptr<base::WrappedWorldEntity>& WE_Base::getWrapper() {
    if(wrapper == nullptr || getIsDisposed() || wrapper->entity == nullptr) {
        // Logic error.
        throw LogicException(LOC);
    }

    if(isMarkedForRemoval) {
        // This can be ok. Remove this check if there is a reason to skip it.
        throw LogicException(LOC);
    }

    return wrapper;
}

sp<base::WrappedWorldEntity>& WE_Base::getWrapperB() {
    if(wrapperB == nullptr || getIsDisposed() || wrapperB->entity == nullptr) {
        // Logic error.
        throw LogicException(LOC);
    }

    if(isMarkedForRemoval) {
        // This can be ok. Remove this check if there is a reason to skip it.
        throw LogicException(LOC);
    }

    return wrapperB;
}

base::TypedEventDispatcher1D<base::EntityEvent>* WE_Base::getEventDispatcher() {
    return &eventDispatcher;
}

std::shared_ptr<IEventListener> WE_Base::addEventListener(
    int type,
    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb)
{
    return eventDispatcher.getDispatcher(type, true)->addListener(cb);
}

void WE_Base::addEventListener(
    int type,
    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    eventDispatcher.getDispatcher(type, true)->addListener(cb, &autoListenersList);
}

void WE_Base::addEventListener(
    int type1, int type2,
    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    addEventListener(type1, cb, autoListenersList);
    addEventListener(type2, cb, autoListenersList);
}

void WE_Base::addEventListener(
    int type1, int type2, int type3,
    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    addEventListener(type1, cb, autoListenersList);
    addEventListener(type2, cb, autoListenersList);
    addEventListener(type3, cb, autoListenersList);
}

void WE_Base::addEventListener(
	ArrayList<int>& eventTypesList,
    std::function<void(IEventListener& eventListener, base::EntityEvent& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	for (int i = 0; i < eventTypesList.count(); i++) {
		int type = eventTypesList.getDirect(i);

		addEventListener(type, cb, autoListenersList);
	}
}

int WE_Base::dispatchEvent(base::EntityEvent& event) {
    int ct = 0;

    base::EventDispatcher<base::EntityEvent>* d = eventDispatcher.getDispatcher(event.type, false);
    if(d != nullptr) {
    	ct += d->dispatch(event);
    }

	if(event.isBubbleAsGWEntityEvent) {
		if (event.entityData != this) {
			// The event entity data was not initialised correctly.
			// This must be correct for bubbling events.
			throw LogicException(LOC);
		}

		ct += gw->dispatchEntityBubbledEvent(event);
	}

	event.clearData();

	return ct;
}

worldGame3D::IPluggableEventsDispatcher* WE_Base::getGWEvents() {
	return &gwEvents;
}

void*& WE_Base::getInEntitiesGroup_entryRef() {
	return inEntitiesGroup_entry;
}

IEntityHandlerBase* WE_Base::getHandler() {
    return handler;
}

worldGame3D::IGameWorldGeneral* WE_Base::getGW() {
	if (gw == nullptr) {
		throw LogicException(LOC);
	}

    return gw;
}

bool WE_Base::getIsInGameWorld() {
	if (gw != nullptr) {
		return true;
	}

	return false;
}

void WE_Base::setGameWorld(worldGame3D::IGameWorldGeneral* gw, IEntityHandlerBase* handler) {
	if (this->gw == gw || (this->gw != nullptr && gw != nullptr)) {
		// Not allowed to set to the same value multiple times or cange from one gw to another without setting to nullptr before.
		throw LogicException(LOC);
	}

	this->gw = gw;
	this->handler = handler;

	syncGamePartConnections();
}

bool WE_Base::getIsActive() {
	return isActiveIncremental >= 1 ? true : false;
}

void WE_Base::setIsActiveIncremental(bool isActive) {
	if (this->isActiveIncremental >= 1 && isActive) {
		// Unable to set enetity more active.
		throw LogicException(LOC);
	}
	this->isActiveIncremental += isActive ? 1 : -1;

	syncGamePartConnections();
}

IListEntry*& WE_Base::getInHandler_markedForRemovalListEntry() {
	return inHandler_markedForRemovalListEntry;
}

bool WE_Base::getIsInGameWorldAndActive() {
	if (gw == nullptr || !getIsActive()) {
		return false;
	}

	return true;
}

bool WE_Base::hasModule(int moduleId) {
    if(hasComponent(moduleId)) {
        return true;
    }

    return false;
}

int WE_Base::getLocalEntityId() {
    return localEntityId;
}

worldGame3D::IPositional3DHandler* WE_Base::getPositional3D() {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	return positional3D;
}

void WE_Base::registerPositional3DHandler(worldGame3D::IPositional3DHandler* positional3DHandler, int priority) {
	if (positional3DHandler == nullptr || positional3D == positional3DHandler) {
		throw LogicException(LOC);
	}

	if (positional3D != nullptr) {
		if (priority == positional3DHandlerPriority) {
			// Priorities must be unique for each module.
			throw LogicException(LOC);
		}
		if (priority < positional3DHandlerPriority) {
			// Priority not high enough.
			return;
		}
	}

	if (positional3D != nullptr) {
		positional3D->setMainIsPositionalHandler(false);

		positional3D = nullptr;
	}

	positional3D = positional3DHandler;
	positional3DHandlerPriority = priority;
	positional3DHandler->setMainIsPositionalHandler(true);
}

void WE_Base::setPos(float x, float y, float z) {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	positional3D->setPos(x, y, z);
}

void WE_Base::setPos(Vector3& pos) {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	positional3D->setPos(pos);
}

void WE_Base::setX(float x) {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	positional3D->setX(x);
}

void WE_Base::setY(float y) {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	positional3D->setY(y);
}

void WE_Base::setZ(float z) {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	positional3D->setZ(z);
}

void WE_Base::setRot(Quaternion& rot) {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	positional3D->setRot(rot);
}

void WE_Base::setRot(Vector3& axis, float degrees) {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	positional3D->setRot(axis, degrees);
}

void WE_Base::setRotEulerZYX(Vector3& axis, float degrees) {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	positional3D->setRotEulerZYX(axis, degrees);
}

Vector3* WE_Base::getScale() {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	return positional3D->getScale();
}

void WE_Base::setScale(float scale) {
	if (positional3D == nullptr) {
		throw LogicException(LOC);
	}

	positional3D->setScale(scale);
}

int WE_Base::getOwnerPlayerId() {
    return ownerPlayerId;
}

void WE_Base::setOwnerPlayerId(int ownerPlayerId) {
    this->ownerPlayerId = ownerPlayerId;
}

bool WE_Base::getIsZoneOwned() {
	return isZoneOwned;
}

void WE_Base::setIsZoneOwned(bool isZoneOwned) {
	this->isZoneOwned = isZoneOwned;
}

void WE_Base::addToContentGroup(std::shared_ptr<worldGame3D::ContentGroup>& contentGroup) {
	IListEntry* listEntry = contentGroup->appendGroupedEntity(this);

	connectedContentGroupsList.append_emplace(contentGroup, listEntry);
}

ArrayList<WE_Base::ConnectedContentGroup>& WE_Base::peekConnectedContentGroupsList() {
	return connectedContentGroupsList;
}

int WE_Base::getSyncedEntityId() {
    return syncedEntityId;
}

void WE_Base::setSyncedEntityId(int syncedEntityId) {
    this->syncedEntityId = syncedEntityId;
}

void WE_Base::registerUninitialisedModule(IEntityModule* mod) {
	if (mod == nullptr) {
		throw LogicException(LOC);
	}

	addComponent(mod, true/*doCreate*/);
}

void WE_Base::createB() {
	for (ListDL<IComponent*>::Entry* listEntry = getComponentsList().getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
		EntityModule* mod = static_cast<EntityModule*>(listEntry->data->componentClassVoidP);

		mod->reservedCreateB();
	}
}

void WE_Base::addComponent(IComponent* component, bool doCreate, int disposeOrderDefault5) {
	IEntityModule* mod = static_cast<IEntityModule*>(component->componentClassVoidP);

	super::addComponent(component, doCreate, disposeOrderDefault5);
}

void WE_Base::removeComponent(IComponent* component, bool doDispose, bool useDelayedDispose) {
	IEntityModule* mod = static_cast<IEntityModule*>(component->componentClassVoidP);

	// If useDelayedDispose is used, this function will be called again later with useDelayedDispose:false and the below reserved dispose is guaranteed to be executed.
	if (doDispose && !useDelayedDispose) {
		//void
	}

	super::removeComponent(component, doDispose, useDelayedDispose);
}

void WE_Base::syncGamePartConnections() {
	if (gw == nullptr || !getIsActive() || !handler->getIsGamePartsEntityUsable()) {
		// Preffered state is disconnected.
		if (isConnectedToGameParts) {
			isConnectedToGameParts = false;

			gwEvents.disconnect();

			for (ListDL<IComponent*>::Entry* listEntry = getComponentsList().getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
				EntityModule* mod = static_cast<EntityModule*>(listEntry->data->componentClassVoidP);

				mod->disconnectFromGamePartsPre();
			}
		}
	} else {
		// Preffered state is connected.
		if (!isConnectedToGameParts) {
			isConnectedToGameParts = true;

			for (ListDL<IComponent*>::Entry* listEntry = getComponentsList().getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
				EntityModule* mod = static_cast<EntityModule*>(listEntry->data->componentClassVoidP);

				mod->connectToGameParts(gw);
			}

			gwEvents.connect(handler->getGWEventDispatcher());
		}
	}
}

std::shared_ptr<base::IWrappedValue> WE_Base::getExtraDataMustExist(std::string& property) {
	std::shared_ptr<base::IWrappedValue> ret = extraDataMap.getDirect(property, nullptr);
	if (ret == nullptr) {
		throw LogicException(LOC);
	}

	return ret;
}

std::shared_ptr<base::IWrappedValue> WE_Base::getExtraDataMustExistB(std::string property) {
	std::shared_ptr<base::IWrappedValue> ret = extraDataMap.getDirect(property, nullptr);
	if (ret == nullptr) {
		throw LogicException(LOC);
	}

	return ret;
}

std::shared_ptr<base::IWrappedValue> WE_Base::getExtraDataOptional(std::string& property) {
	return extraDataMap.getDirect(property, nullptr);
}

std::shared_ptr<base::IWrappedValue> WE_Base::getExtraDataOptionalB(std::string property) {
	return extraDataMap.getDirect(property, nullptr);
}

void WE_Base::setExtraData(std::string& property, std::shared_ptr<base::IWrappedValue> value) {
	extraDataMap.putReference(property, value);
}

void WE_Base::setExtraDataB(std::string property, std::shared_ptr<base::IWrappedValue> value) {
	extraDataMap.putReference(property, value);
}

void WE_Base::disposeMainPre() {
	dispatchEvent(*EVENT_onRemoved_pre->setEntityData(this));

	super::disposeMainPre();
}

void WE_Base::disposeMain() {
	if (inEntitiesGroup_entry != nullptr) {
		// This entity is still in a management list, remove it from there first.
		throw LogicException(LOC);
	}

	// Remove from content groups.
	for (int i = 0; i < connectedContentGroupsList.count(); i++) {
		ConnectedContentGroup& connectedCGroup = connectedContentGroupsList.getReference(i);
		connectedCGroup.entryInContentGroup->remove();
	}
	connectedContentGroupsList.clear();

	if (wrapper != nullptr) {
		if (wrapper->entity != nullptr) {
			// Also do this here because entities may be manually removed (eg: on area change).
			wrapper->entity = nullptr;
		}

		wrapper = nullptr;
	}
	if (wrapperB != nullptr) {
		if (wrapperB->entity != nullptr) {
			// Also do this here because entities may be manually removed (eg: on area change).
			wrapperB->entity = nullptr;
		}

		wrapperB = nullptr;
	}

	for (ListDL<IComponent*>::Entry* listEntry = getComponentsList().getFirst(); listEntry != nullptr; listEntry = listEntry->next) {
		EntityModule* mod = static_cast<EntityModule*>(listEntry->data->componentClassVoidP);

        mod->reservedDisposePre();
    }
	// The modules are fully disposed in the super call, the above just calls the custom disposePre;

	super::disposeMain();
}

WE_Base::~WE_Base() {
	//void
}
