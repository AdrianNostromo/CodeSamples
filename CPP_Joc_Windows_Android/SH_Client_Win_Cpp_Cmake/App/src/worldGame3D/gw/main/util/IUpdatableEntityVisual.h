#pragma once

#include <base/gh.h>

class IWorldEntity;
class IListEntry;

class IUpdatableEntityVisual {
    pub virtual IWorldEntity* getEntity() = 0;
    pub virtual void updateVisual() = 0;

    pub virtual IListEntry*& getInHandler_VisualUpdateListEntryRef() = 0;

    pub virtual ~IUpdatableEntityVisual() {}
};
