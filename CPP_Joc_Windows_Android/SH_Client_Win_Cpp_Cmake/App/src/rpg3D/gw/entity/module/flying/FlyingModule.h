#pragma once

#include <base/gh.h>
#include "FM_GeneralFlight.h"

namespace rpg3D {
class FlyingModule : public FM_GeneralFlight {priv typedef FM_GeneralFlight super;pub dCtor(FlyingModule);
    pub explicit FlyingModule(
		IWorldEntity* entity,
		FlyingTemplate* t);

    pub ~FlyingModule() override;
};
};
