#pragma once

#include <base/gh.h>
#include "MEMT_TurnToTargetAngle.h"

namespace rpg3D {
class MEMTurn : public MEMT_TurnToTargetAngle {priv typedef MEMT_TurnToTargetAngle super; pub dCtor(MEMTurn);
    pub explicit MEMTurn(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);

    pub ~MEMTurn() override;
};
};
