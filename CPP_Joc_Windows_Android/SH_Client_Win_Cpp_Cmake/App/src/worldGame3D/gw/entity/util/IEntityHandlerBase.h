#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/handler/IHandler_GameWorld.h>
#include <worldGame3D/gw/entity/template/EntityTemplate.h>
#include <base/math/Vector3.h>
#include <base/math/Quaternion.h>
#include <worldGame3D/gw/main/util/IScreenTouch2DListener.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>

class IWorldEntity;
class EntityRemoveReason;
class IUpdatableEntityVisual;
namespace base {
    class GWEvent;
};
namespace worldGame3D {
    class IGameWorldGeneral;
    class IPlatformInteractorModule;
    class IPlatformModule;
    class ContentGroup;
}

class IEntityHandlerBase {
    pub virtual worldGame3D::IGameWorldGeneral* getGW() = 0;
    pub virtual base::TypedEventDispatcher1D<base::GWEvent>* getGWEventDispatcher() = 0;
    pub virtual IHandler_GameWorld* getHandler() = 0;

    pub virtual void addDrawableModuleToVisualUpdate(IUpdatableEntityVisual* targetModule) = 0;
    pub virtual IWorldEntity* createEntity(
        EntityTemplate* entityTemplate,
        Vector3& wPos, Quaternion* rot, float* cardinalRotation,
        std::shared_ptr<std::string> instanceId, int ownerPlayerId,
        std::shared_ptr<ExtraData> extraData,
        bool isZoneOwnedEntity, ArrayList<std::shared_ptr<worldGame3D::ContentGroup>>* manualContentGroupsList, IWorldEntity* creatorEntity,
        std::function<void(IWorldEntity* entity)> entityPostInitCb,
        bool isActive = true
    ) = 0;
    pub virtual void markEntityForRemoval(
            IWorldEntity* entity,
            EntityRemoveReason* removeReason, bool isRequestFromServer, bool disposeAfterRemoval=true
    ) = 0;
    pub virtual IWorldEntity* getEntityWithLocalEntityId(int localEntityId) = 0;

    pub virtual int countChopAxesHeadingTowardsTreeWithLocalEntityId(int treeLocalEntityId) = 0;

    // This is used to disconnect entities from game parts when the gParts may change (on zone changes).
    pub virtual bool getIsGamePartsEntityUsable() = 0;

    pub virtual ~IEntityHandlerBase() {}
};
