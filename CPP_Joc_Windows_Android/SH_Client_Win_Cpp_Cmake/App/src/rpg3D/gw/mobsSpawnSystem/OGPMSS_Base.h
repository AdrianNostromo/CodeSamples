#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IOGPMobsSpawnSystem.h"

namespace rpg3D {
class OGPMSS_Base : public GamePart, virtual public IOGPMobsSpawnSystem {priv typedef GamePart super;pub dCtor(OGPMSS_Base);
    pub explicit OGPMSS_Base(base::IGameWorld* gw);

    pub ~OGPMSS_Base() override;
};
};
