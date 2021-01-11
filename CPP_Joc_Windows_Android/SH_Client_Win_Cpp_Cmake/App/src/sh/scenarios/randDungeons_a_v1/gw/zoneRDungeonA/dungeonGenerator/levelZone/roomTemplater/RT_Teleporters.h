#pragma once

#include <base/gh.h>
#include "RT_Enclosure.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class RT_Teleporters : public RT_Enclosure {priv typedef RT_Enclosure super;pub dCtor(RT_Teleporters);
    pub explicit RT_Teleporters();

    prot void bakeTeleporters();

    pub ~RT_Teleporters() override;
};
};
};
