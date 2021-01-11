#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IOGPConditionalEntities.h"

namespace rpg3D {
class OGPCE_Base : public GamePart, virtual public IOGPConditionalEntities {priv typedef GamePart super;pub dCtor(OGPCE_Base);
    pub explicit OGPCE_Base(base::IGameWorld* gw);

    pub ~OGPCE_Base() override;
};
};
