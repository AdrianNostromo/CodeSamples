#pragma once

#include <base/gh.h>
#include "../util/ZoneGridCell.h"
#include <base/list/Array2D.h>
#include "../../config/CfgDungeonGenerator.h"
#include <base/math/Vector2Int.h>
#include <memory>
#include <base/math/Bounds2DInt.h>

namespace randDungeons_a_v1 {
namespace RDungeonA {
class RG_Base {pub dCtor(RG_Base);
    pub enum RoomGrowType {LeastRoad, MostRoad};

    prot int roomIndex = -1;
    prot Vector2Int startZonePos{};
    // This may be replaced with a special sizer that can handle multiple sizes and custom bitMask shapes (eg. circle, custom_shapes).
    prot int preferredRoomSize_smallAxis;
    prot int preferredRoomSize_bigAxis;
    prot Array2D<ZoneGridCell>* zoneGrid = nullptr;
    prot Random* rng;

    pub explicit RG_Base();

    pub virtual void clear();

    pub virtual void autoGrowRoom(
        int roomIndex, Vector2Int& startZonePos, int preferredRoomSize_smallAxis, int preferredRoomSize_bigAxis,
        Array2D<ZoneGridCell>* zoneGrid, RoomGrowType roomGrowType, Bounds2DInt& oFinalRoomBounds, Random* rng);

    pub virtual ~RG_Base();
};
};
};
