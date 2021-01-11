#pragma once

#include <base/gh.h>
#include "MEMTurn.h"

namespace rpg3D {
class MEMM_Base : public MEMTurn {priv typedef MEMTurn super; pub dCtor(MEMM_Base);
    prot static int moveRequestTypeCounter;

    pub explicit MEMM_Base(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);

    prot static int newMoveRequestType();

    pub ~MEMM_Base() override;
};
};
