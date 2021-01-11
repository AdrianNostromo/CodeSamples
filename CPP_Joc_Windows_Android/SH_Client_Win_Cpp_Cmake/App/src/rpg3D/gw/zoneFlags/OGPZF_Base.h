#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IOGPZoneFlags.h"

namespace rpg3D {
class OGPZF_Base : public GamePart, virtual public IOGPZoneFlags {priv typedef GamePart super;pub dCtor(OGPZF_Base);
    pub explicit OGPZF_Base(base::IGameWorld* gw);

    pub ~OGPZF_Base() override;
};
};
