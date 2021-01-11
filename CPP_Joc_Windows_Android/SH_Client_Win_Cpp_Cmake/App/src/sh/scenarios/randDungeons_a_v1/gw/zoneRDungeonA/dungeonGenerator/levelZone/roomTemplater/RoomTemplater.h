#pragma once

#include <base/gh.h>
#include "RT_Mobs.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class RoomTemplater final/*Note1001. calls_the_reserved_dispose*/ : public RT_Mobs {priv typedef RT_Mobs super;pub dCtor(RoomTemplater);
    pub explicit RoomTemplater();

    pub void generate(
        std::shared_ptr<CfgDungeonGenerator>& config, Vector3& roomOriginZonePos, std::shared_ptr<Result_RoomElementsPopulator> in_roomElementsPopulatorResult,
        int zoneIndex) override;

    pub ~RoomTemplater() override;
};
};
};
