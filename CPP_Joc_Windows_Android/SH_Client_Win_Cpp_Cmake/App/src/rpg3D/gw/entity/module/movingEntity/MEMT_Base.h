#pragma once

#include <base/gh.h>
#include "MEMBase.h"

namespace rpg3D {
class MEMT_Base : public MEMBase {priv typedef MEMBase super;pub dCtor(MEMT_Base);
    prot static int turnRequestTypeCounter;
    
    pub explicit MEMT_Base(IWorldEntity* entity, rpg3D::MovingEntityTemplate* t);

    prot static int newTurnRequestType();

    pub ~MEMT_Base() override;
};
};
