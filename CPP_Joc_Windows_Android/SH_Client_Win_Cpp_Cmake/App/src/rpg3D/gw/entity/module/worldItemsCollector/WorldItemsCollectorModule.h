#pragma once

#include <base/gh.h>
#include "WICM_Base.h"

namespace rpg3D {
class WorldItemsCollectorModule : public WICM_Base {priv typedef WICM_Base super;pub dCtor(WorldItemsCollectorModule);
    pub explicit WorldItemsCollectorModule(
		IWorldEntity* entity,
		WorldItemsCollectorTemplate* t);

    pub ~WorldItemsCollectorModule() override;
};
};
