#pragma once

#include <base/gh.h>
#include <base/list/ListDL.h>
#include <string>
#include <base/MM.h>

class IWorldEntity;
class EntityRemoveReason;

namespace worldGame3D {
class ContentGroup {pub dCtor(ContentGroup);
    pub ListDL<IWorldEntity*> entitiesList{LOC};

    pub explicit ContentGroup();

    pub IListEntry* appendGroupedEntity(IWorldEntity* entity);

    pub void setAllEntitiesEnabledState(bool isEnabled);

    pub ~ContentGroup();
};
};
