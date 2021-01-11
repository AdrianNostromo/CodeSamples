#pragma once

#include <base/gh.h>
#include <memory>
#include <base/math/Random.h>
#include "../config/CfgDungeonGenerator.h"
#include "../../dungeonContent/DungeonContent.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class LZG_Base {pub dCtor(LZG_Base);
    prot std::shared_ptr<CfgDungeonGenerator> config;
    prot int zoneIndex;
    prot Random* rng;

    prot std::shared_ptr<DungeonContent::LevelZone> levelZone;

    pub explicit LZG_Base(
        std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
        Random* rng);

    pub virtual ~LZG_Base();
};
};
};
