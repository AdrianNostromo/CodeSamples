#pragma once

#include <base/gh.h>
#include "RG_AlgoRegionRoomsFinder.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class RG_Grower : public RG_AlgoRegionRoomsFinder {priv typedef RG_AlgoRegionRoomsFinder super;pub dCtor(RG_Grower);
    pub explicit RG_Grower();

    pub void autoGrowRoom(
        int roomIndex, Vector2Int& startZonePos, int preferredRoomSize_smallAxis, int preferredRoomSize_bigAxis,
        Array2D<ZoneGridCell>* zoneGrid, RoomGrowType roomGrowType, Bounds2DInt& oFinalRoomBounds, Random* rng) override;

    pub ~RG_Grower() override;
};
};
};
