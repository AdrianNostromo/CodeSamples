#pragma once

#include <base/gh.h>
#include "REP_Enclosure.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class REP_Teleporters : public REP_Enclosure {priv typedef REP_Enclosure super;pub dCtor(REP_Teleporters);
    pub explicit REP_Teleporters();

    prot void placeTeleporters();

    pub ~REP_Teleporters() override;
};
};
};
