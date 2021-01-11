#pragma once

#include <base/gh.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>

class IWorldEntity;
namespace worldGame3D {
    class EntGroupsEvent;
};

namespace worldGame3D {
class IEntsGroup {
    pub static EntGroupsEvent* EGV_EntityAdded;
    pub static EntGroupsEvent* EGV_EntityRemovePre;
    pub static EntGroupsEvent* EGV_EntityRemoved;

    pub virtual int getUid() = 0;

    pub virtual base::TypedEventDispatcher1D<EntGroupsEvent>& getEvents() = 0;

    pub virtual int getEntitiesCount() = 0;
    pub virtual bool hasEntity(IWorldEntity* entity) = 0;
    pub virtual void addEntity(IWorldEntity* entity) = 0;
    pub virtual void removeEntity(IWorldEntity* entity, bool disposeTheEntity) = 0;
    pub virtual void disposeAllEntities() = 0;

    pub virtual ~IEntsGroup() = default;
};
};
