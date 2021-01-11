#pragma once

#include <base/gh.h>
#include "REP_Base.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class REP_Floors : public REP_Base {priv typedef REP_Base super;pub dCtor(REP_Floors);
    pub explicit REP_Floors();

    prot void createFloorTilingGrid();

    pub ~REP_Floors() override;
};
};
};
