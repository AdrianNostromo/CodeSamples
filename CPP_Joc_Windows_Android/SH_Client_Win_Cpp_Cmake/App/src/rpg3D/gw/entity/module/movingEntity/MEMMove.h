#pragma once

#include <base/gh.h>
#include "MEMM_ToTarget.h"

namespace rpg3D {
class MEMMove : public MEMM_ToTarget {priv typedef MEMM_ToTarget super; pub dCtor(MEMMove);
    pub explicit MEMMove(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);

    pub ~MEMMove() override;
};
};
