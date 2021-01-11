#pragma once

#include <base/gh.h>
#include "RG_Grower.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class RoomGrower : public RG_Grower {priv typedef RG_Grower super;pub dCtor(RoomGrower);
    pub explicit RoomGrower();

    pub ~RoomGrower() override;
};
};
};
