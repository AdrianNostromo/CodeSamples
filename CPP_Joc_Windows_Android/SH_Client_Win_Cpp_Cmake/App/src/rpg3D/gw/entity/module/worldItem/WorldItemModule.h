#pragma once

#include <base/gh.h>
#include "WIM_Base.h"

namespace rpg3D {
class WorldItemModule : public WIM_Base {priv typedef WIM_Base super;pub dCtor(WorldItemModule);
    pub explicit WorldItemModule(
		IWorldEntity* entity,
		WorldItemTemplate* t);

    pub ~WorldItemModule() override;
};
};
