#pragma once

#include <base/gh.h>
#include "RT_Floor.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class RT_Enclosure : public RT_Floor {priv typedef RT_Floor super;pub dCtor(RT_Enclosure);
    pub explicit RT_Enclosure();

    prot void bakeEnclosure();
    
    pub ~RT_Enclosure() override;
};
};
};
