#include "GPMEntities.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/WrappedWorldEntity.h>
#include <base/util/StringUtil.h>
#include <worldGame3D/gw/main/util/IUpdatableEntityVisual.h>

using namespace base;

GPMEntities::GPMEntities(base::IGameWorld* gw)
    : super(gw)
{
    gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_visualsUpdate->type,
		std::bind(&GPMEntities::onGWEvent_gameLoop_visualsUpdate, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
    gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_entitiesRemoval->type,
		std::bind(&GPMEntities::onGWEvent_gameLoop_entitiesRemoval, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPMEntities::addDrawableModuleToVisualUpdate(IUpdatableEntityVisual* drawableModule) {
    if (drawableModule->getInHandler_VisualUpdateListEntryRef() != nullptr) {
        return;
    }

    //asd_x;// this method requires a lot of extra checks in case the entity is removed, tool_visual entity changed or re-added to the gWorld. A different logic may be preffered.
    drawableModule->getInHandler_VisualUpdateListEntryRef() = drawableEntityModulesToUpdateList.appendEmplace(drawableModule->getEntity()->getWrapper(), drawableModule);
}

void GPMEntities::onGWEvent_gameLoop_visualsUpdate(IEventListener& eventListener, base::GWEvent& event) {
    while(drawableEntityModulesToUpdateList.count() > 0) {
        ListDL<DrawableToUpdateEntry>::Entry* listEntry = drawableEntityModulesToUpdateList.getFirst();
        DrawableToUpdateEntry* entryToUpdate = &listEntry->data;

        // Skip if disposed. Still update if marked for removal.
        if (entryToUpdate->wEntity->entity == nullptr) {
            listEntry->remove();

            continue;
        }
        if (!entryToUpdate->wEntity->entity->getIsInGameWorldAndActive()
            || entryToUpdate->wEntity->entity != entryToUpdate->drawableModule->getEntity()/*entity_changed_probably_tool_item_transferred_to_other_entity*/
            || entryToUpdate->wEntity->entity->getIsMarkedForRemoval()
            || entryToUpdate->drawableModule->getInHandler_VisualUpdateListEntryRef() != listEntry)
        {
            entryToUpdate->drawableModule->getInHandler_VisualUpdateListEntryRef() = nullptr;

            listEntry->remove();

            continue;
        }

        entryToUpdate->drawableModule->updateVisual();
        entryToUpdate->drawableModule->getInHandler_VisualUpdateListEntryRef() = nullptr;

        if(entryToUpdate->wEntity->entity->getIsDisposed()) {
            throw LogicException(LOC);
        }

        listEntry->remove();
    }
}

IWorldEntity* GPMEntities::getEntityWithLocalEntityId(int localEntityId) {
    for(int i = entitiesList.size()-1; i>=0; i--) {
        IWorldEntity* entity = entitiesList.getDirect(i);
        if (entity->getLocalEntityId() == localEntityId) {
            return entity;
        }
    }

    return nullptr;
}

ArrayList<IWorldEntity*>* GPMEntities::peekEntitiesList() {
    return &entitiesList;
}

IWorldEntity* GPMEntities::getEntityWithSyncedEntityId(int syncedEntityId) {
    for(int i = entitiesList.size()-1; i>=0; i--) {
        IWorldEntity* entity = *entitiesList.getPointer(i);
        if (entity->getSyncedEntityId() == syncedEntityId) {
            return entity;
        }
    }

    return nullptr;
}

void GPMEntities::addWorldEntity(IWorldEntity* entity, std::shared_ptr<std::string> instanceId) {
    if (entity->getIsInGameWorld() || entity->getIsMarkedForRemoval()) {
        // Entity must be removed from the gw before it can be re-added.
        throw LogicException(LOC);
    }
    
    entitiesList.appendDirect(entity);

    if(instanceId != nullptr) {
        entity->setInstanceId(instanceId);

        addEntityToInstanceIdTracking(entity);
    }

    entity->setGameWorld(gw, this);

    gw->dispatchEvent(*GWEvent_onEntityAdded->setEntityData(entity));
}

void GPMEntities::onGWEvent_gameLoop_entitiesRemoval(IEventListener& eventListener, base::GWEvent& event) {
    while(this->markedForRemovalEntities.count() > 0) {
        ListDL<MarkedForRemovalEntity>::Entry* entry = this->markedForRemovalEntities.getFirst();

        IWorldEntity* entity = entry->data.entity;
        bool disposeAfterRemoval = entry->data.disposeAfterRemoval;

        // Call this here because removeEntity...) also removes it.
        entry->remove();

        if (entity->getIsDisposed()) {
            throw LogicException(LOC);
        }

        entity->getInHandler_markedForRemovalListEntry() = nullptr;

        if(!entity->getIsMarkedForRemoval()) {
            throw LogicException(LOC);
        }
        
        this->removeEntity(entity, entity->getMarkedForRemovalReason());
    }
}

// NOTE : This should be called only on local entities and when the server send a update command to remove it.
/// Don't remove synced entities using local logic.
void GPMEntities::markEntityForRemoval(IWorldEntity* entity, EntityRemoveReason* removeReason, bool isRequestFromServer, bool disposeAfterRemoval) {
    // Ignore remove requests made from local logic to synced entities.
    if (entity->getIsMarkedForRemoval()) {
        throw LogicException(LOC);
    }

    if(isRequestFromServer || entity->getSyncedEntityId() < 0) {
        entity->getInHandler_markedForRemovalListEntry() = this->markedForRemovalEntities.appendEmplace(entity, disposeAfterRemoval);

        entity->fromHandler_setIsMarkedForRemoval(removeReason);
    }
}

bool GPMEntities::removeEntity(IWorldEntity* entity, EntityRemoveReason* removeReason, bool disposeAfterRemoval) {
    int i = entitiesList.indexOf(entity);
    if(i < 0) {
        // This used to return false, avoid that type pf non-critical permissive logic.
        throw LogicException(LOC);
    }

    gw->dispatchEvent(*GWEvent_onEntityRemoved_Pre->setEntityData(entity));

    // If entity is marked for removal, just skip that in this case.
    if (entity->getInHandler_markedForRemovalListEntry() != nullptr) {
        entity->getInHandler_markedForRemovalListEntry()->remove();
        entity->getInHandler_markedForRemovalListEntry() = nullptr;
    }

    entity->setGameWorld(nullptr, nullptr);
    entity->clearIsMarkedForRemoval();

    entitiesList.remove(i);

    if(entity->getInstanceId() != nullptr) {
        removeEntityFromInstanceIdTracking(entity);

        entity->setInstanceId(nullptr);
    }

    if (disposeAfterRemoval) {
        entity->reservedDisposeMain();
    }

    return true;
}

IWorldEntity* GPMEntities::getEntityWithInstanceId(std::string& instanceId, bool mustExist) {
    std::shared_ptr<ArrayList<IWorldEntity*>> instanceEntitiesList = getEntitiesListWithInstanceId(instanceId);

    if(instanceEntitiesList == nullptr) {
        if (mustExist) {
            throw LogicException(LOC);
        }

        return nullptr;
    }

    if(instanceEntitiesList->count() != 1) {
        // This function can be used only if maximum 1 entity with the instance id exists.
        // For known multiple entities that use the same instace id, use the list returning version: getEntitiesListWithInstanceId(...).
        throw LogicException(LOC);
    }

    return instanceEntitiesList->getDirect(0);
}

bool GPMEntities::isAnyEntityWithInstanceId(std::string& instanceId) {
    std::shared_ptr<ArrayList<IWorldEntity*>> filteredEntitiesList = instancedEntitiesMap.getDirect(instanceId, nullptr);
    if (filteredEntitiesList != nullptr && filteredEntitiesList->count() > 0) {
        return true;
    }

    return false;
}

std::shared_ptr<ArrayList<IWorldEntity*>> GPMEntities::getEntitiesListWithInstanceId(std::string& instanceId) {
    return instancedEntitiesMap.getDirect(instanceId, nullptr);
}

void GPMEntities::addEntityToInstanceIdTracking(IWorldEntity* entity) {
    std::shared_ptr<std::string> instanceId = entity->getInstanceId();

    if(instanceId == nullptr) {
        // The entity must have a instanceId.
        throw LogicException(LOC);
    }

    std::shared_ptr<ArrayList<IWorldEntity*>> instanceEntitiesList = getEntitiesListWithInstanceId(*instanceId);
    if(instanceEntitiesList == nullptr) {
        // Create and push the list.
        instanceEntitiesList = std::make_shared<ArrayList<IWorldEntity*>>();

        instancedEntitiesMap.putDirect(*instanceId, instanceEntitiesList);
    }

    instanceEntitiesList->appendDirect(entity);
}

void GPMEntities::removeEntityFromInstanceIdTracking(IWorldEntity* entity) {
    std::shared_ptr<std::string> instanceId = entity->getInstanceId();

    if(instanceId == nullptr) {
        // The entity must have a instanceId.
        throw LogicException(LOC);
    }

    std::shared_ptr<ArrayList<IWorldEntity*>> instanceEntitiesList = getEntitiesListWithInstanceId(*instanceId);
    if(instanceEntitiesList == nullptr || !instanceEntitiesList->remove(entity)) {
        throw LogicException(LOC);
    }
    if(instanceEntitiesList->count() == 0) {
        instancedEntitiesMap.removeMustExist(*instanceId);
    }
}

int GPMEntities::countChopAxesHeadingTowardsTreeWithLocalEntityId(int treeLocalEntityId) {
    int ct = 0;

    assert(false);
    /*asdAA
    ArrayList<WorldEntity*>* entitiesList = peekEntitiesList();
    for(int i=0;i<entitiesList->size();i++) {
        WorldEntity* entity = entitiesList->get(i);

        TreeChopProjectileModule treeChopProjectile = entity->getModuleMustExist(TreeChopProjectileTemplate.MODULE_ID);

        if(treeChopProjectile != nullptr && treeChopProjectile.getTargetTreeLocalEntityId() == treeLocalEntityId) {
            ct++;
        }
    }*/

    return ct;
}

bool GPMEntities::getIsGamePartsEntityUsable() {
    return isGamePartsEntityUsable;
}

void GPMEntities::removeAllZoneOwnedEntities() {
    for (int i = entitiesList.size() - 1; i >= 0; i--) {
        IWorldEntity* entity = *entitiesList.getPointer(i);

        if (entity->getIsZoneOwned()) {
            this->removeEntity(entity, &EntityRemoveReason::CODE_DISPOSED);
        }
    }
}

void GPMEntities::connectAllEntitiesToGameParts() {
    if (isGamePartsEntityUsable) {
        // This is not an error because this is a optional mechanism used only by content zones.
        return;
    }
    isGamePartsEntityUsable = true;

    for (int i = entitiesList.size() - 1; i >= 0; i--) {
        IWorldEntity* entity = *entitiesList.getPointer(i);

        entity->syncGamePartConnections();
    }
}

void GPMEntities::disconnectAllEntitiesFromGameParts() {
    if (!isGamePartsEntityUsable) {
        // This is not an error because this is a optional mechanism used only by content zones.
        return;
    }
    isGamePartsEntityUsable = false;
    
    for (int i = entitiesList.size() - 1; i >= 0; i--) {
        IWorldEntity* entity = *entitiesList.getPointer(i);

        entity->syncGamePartConnections();
    }
}

void GPMEntities::disposePre() {
    for(int i = entitiesList.size()-1; i>=0; i--) {
        IWorldEntity* entity = *entitiesList.getPointer(i);

        this->removeEntity(entity, &EntityRemoveReason::CODE_DISPOSED);
        if (entitiesList.size() != i) {
            // The entity remove didn't occur correctly.
            throw LogicException(LOC);
        }

    }
    if (entitiesList.size() != 0) {
        // The entities remove didn't occur correctly.
        throw LogicException(LOC);
    }

    super::disposePre();
}

GPMEntities::~GPMEntities() {
    //void
}
