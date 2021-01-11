#pragma once

#include <base/gh.h>
#include "REP_Teleporters.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class REP_Mobs : public REP_Teleporters {priv typedef REP_Teleporters super;pub dCtor(REP_Mobs);
    pub explicit REP_Mobs();

    prot void placeMobsGroup();

    pub ~REP_Mobs() override;
};
};
};
