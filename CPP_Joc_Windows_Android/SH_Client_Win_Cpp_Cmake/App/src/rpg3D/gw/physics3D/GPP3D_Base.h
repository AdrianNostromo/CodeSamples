#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGamePartPhysics3D.h"

namespace rpg3D {
class GPP3D_Base : public GamePart, virtual public IGamePartPhysics3D {priv typedef GamePart super;pub dCtor(GPP3D_Base);
    pub explicit GPP3D_Base(base::IGameWorld* gw);

    pub ~GPP3D_Base() override;
};
};
