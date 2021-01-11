#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <memory>
#include "util/Result_RoomTemplater.h"
#include "../roomElementsPopulator/util/Result_RoomElementsPopulator.h"
#include "../../config/CfgDungeonGenerator.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class RT_Base : public base::Object {priv typedef base::Object super;pub dCtor(RT_Base);
    prot std::shared_ptr<CfgDungeonGenerator> config;
    prot std::shared_ptr<Result_RoomElementsPopulator> in_roomElementsPopulatorResult = nullptr;
    prot int zoneIndex = -1;

    pub std::shared_ptr<Result_RoomTemplater> result = nullptr;

    pub explicit RT_Base();

    pub virtual void generate(
        std::shared_ptr<CfgDungeonGenerator>& config, Vector3& roomOriginZonePos, std::shared_ptr<Result_RoomElementsPopulator> in_roomElementsPopulatorResult,
        int zoneIndex);

    pub ~RT_Base() override;
};
};
};
