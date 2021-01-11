#pragma once

#include <base/gh.h>
#include <base/math/Vector2Int.h>
#include <base/list/Array1D.h>

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
struct ZoneGridCell {
    pub class RoadDir {
        pub static const int xPositive;
        pub static const int xNegative;
        pub static const int yPositive;
        pub static const int yNegative;
        
        // This is used to allow for unconnected road zoneGCells.
        // All roads have this set.
        pub static const int Placeholder;
    };
    pub struct DirInfo {
        int roadDir;
        Vector2Int neighbourOffset;
    };
    pub static Array1D<DirInfo> directionsInfo;

    pub short roomIndex = -1;

    pub char roadDirBitMap = 0;

    pub short pathingDists[2]{ -1, -1 };
};
};
};
