#pragma once

#include <base/gh.h>
#include "RG_RoomEnclosureGeneration.h"

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class RG_GeneratedToEntities : public RG_RoomEnclosureGeneration {priv typedef RG_RoomEnclosureGeneration super;pub dCtor(RG_GeneratedToEntities);
    pub explicit RG_GeneratedToEntities(sp<CfgRoomGenerator> config, int seed = -1);

    prot void populateFloorsEntities();
    prot void populateWallsEntities();

    pub ~RG_GeneratedToEntities() override;
};
};
};
