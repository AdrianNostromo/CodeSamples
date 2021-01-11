#include "GPMEntitiesCreator.h"
#include <worldGame3D/gw/entity/util/IPositional3DHandler.h>
#include <worldGame3D/gw/entity/WorldEntity.h>

using namespace base;

GPMEntitiesCreator::GPMEntitiesCreator(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

base::IGameWorld* GPMEntitiesCreator::getGW() {
    return gw;
}

base::TypedEventDispatcher1D<base::GWEvent>* GPMEntitiesCreator::getGWEventDispatcher() {
    return gw->getEventDispatcher();
}

IWorldEntity* GPMEntitiesCreator::createEntity(
    EntityTemplate* entityTemplate,
    Vector3& wPos, Quaternion* rot, float* cardinalRotation,
    std::shared_ptr<std::string> instanceId, int ownerPlayerId,
    std::shared_ptr<ExtraData> extraData,
    // Only one of contentGroupsList or creatorEntity may be provided. They serve the same purpose.
    bool isZoneOwnedEntity, ArrayList<std::shared_ptr<worldGame3D::ContentGroup>>* manualContentGroupsList, IWorldEntity* creatorEntity,
    std::function<void(IWorldEntity* entity)> entityPostInitCb,
    bool isActive)
{
    if(entityTemplate == nullptr) {
        throw LogicException(LOC);
    }

    int localEntityId = ++localEntityIdIncrementor;

    WorldEntity* entity = createEntityObject(
        entityTemplate,
        localEntityId,
        extraData
    );

    if (isActive) {
        entity->setIsActiveIncremental(true);
    }

    entity->setPos(wPos);
    if (rot != nullptr) {
        if (cardinalRotation != nullptr) {
            throw LogicException(LOC);
        }

        entity->getPositional3D()->setRot(*rot);
    } else if (cardinalRotation != nullptr) {
        if (rot != nullptr) {
            throw LogicException(LOC);
        }
        entity->getPositional3D()->setCardinalRotation(*cardinalRotation);
    } else {
        // Rotation not speciffied.
        throw LogicException(LOC);
    }

    entity->createB();

    entity->setOwnerPlayerId(ownerPlayerId);

    entity->setIsZoneOwned(isZoneOwnedEntity);

    if (manualContentGroupsList != nullptr) {
        if (creatorEntity != nullptr) {
            throw LogicException(LOC);
        }

        for (int i = 0; i < manualContentGroupsList->count(); i++) {
            std::shared_ptr<worldGame3D::ContentGroup>& cGroup = manualContentGroupsList->getReference(i);
            entity->addToContentGroup(cGroup);
        }
    } else if (creatorEntity != nullptr) {
        if (manualContentGroupsList != nullptr) {
            throw LogicException(LOC);
        }

        ArrayList<WorldEntity::ConnectedContentGroup>& connectedContentGroupsList = creatorEntity->peekConnectedContentGroupsList();
        for (int i = 0; i < connectedContentGroupsList.count(); i++) {
            WorldEntity::ConnectedContentGroup& connectedCGroup = connectedContentGroupsList.getReference(i);
            entity->addToContentGroup(connectedCGroup.contentGroup);
        }
    }

    if(entityPostInitCb) {
        entityPostInitCb(entity);
    }

    addWorldEntity(entity, instanceId);

    return entity;
}

WorldEntity* GPMEntitiesCreator::createEntityObject(EntityTemplate* t, int localEntityId, std::shared_ptr<ExtraData> extraData) {
    WorldEntity* entity = new WorldEntity(t, localEntityId, extraData);

    for(int i=0;i<entity->t->moduleTemplatesList.size();i++) {
        BasicModuleTemplate* subTemplate = *entity->t->moduleTemplatesList.getPointer(i);

        IEntityModule* mod = subTemplate->createNewModuleIfExists(
			entity, subTemplate, extraData, this
		);
        if(mod != nullptr) {
            entity->registerUninitialisedModule(mod);
        }
    }

    return entity;
}

GPMEntitiesCreator::~GPMEntitiesCreator() {
    //void
}
