#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGPUserEntitiesController.h"

namespace rpg3D {
class GPUEC_Base : public GamePart, virtual public IGPUserEntitiesController {priv typedef GamePart super;pub dCtor(GPUEC_Base);
    pub explicit GPUEC_Base(base::IGameWorld* gw);

    pub ~GPUEC_Base() override;
};
};
