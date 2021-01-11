#pragma once

#include <base/gh.h>
#include "RT_Teleporters.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class RT_Mobs : public RT_Teleporters {priv typedef RT_Teleporters super;pub dCtor(RT_Mobs);
    pub explicit RT_Mobs();

    prot void bakeMobs();

    pub ~RT_Mobs() override;
};
};
};
