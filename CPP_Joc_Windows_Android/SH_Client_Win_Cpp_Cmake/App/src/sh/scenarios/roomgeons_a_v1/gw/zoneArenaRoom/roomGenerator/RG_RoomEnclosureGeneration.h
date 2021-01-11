#pragma once

#include <base/gh.h>
#include "RG_Prefabs.h"
#include <base/list/ListDL.h>

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class RG_RoomEnclosureGeneration : public RG_Prefabs {priv typedef RG_Prefabs super;pub dCtor(RG_RoomEnclosureGeneration);
    prot ListDL<Door> doorsList{};

    pub explicit RG_RoomEnclosureGeneration(sp<CfgRoomGenerator> config, int seed = -1);

    prot void generateWallsAndDoors();

    pub ~RG_RoomEnclosureGeneration() override;
};
};
};
