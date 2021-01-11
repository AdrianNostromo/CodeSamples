#pragma once

#include <base/gh.h>
#include "RT_Base.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class RT_Floor : public RT_Base {priv typedef RT_Base super;pub dCtor(RT_Floor);
    pub explicit RT_Floor();

    prot void bakeFloor();

    pub ~RT_Floor() override;
};
};
};
