#pragma once

#include <base/gh.h>
#include "RG_RoomGrid.h"
#include <base/math/Vector2Int.h>
#include <base/list/Array2D.h>

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class RG_Prefabs : public RG_RoomGrid {priv typedef RG_RoomGrid super;pub dCtor(RG_Prefabs);
    prot class PrefabTile {
        pub signed char roomInfo;
        // -2; will use checker pattern.
        pub signed char tilingTileIndex;
    };
    prot class RoomConfig {
        pub bool canPlaceDoorsWithOtherRooms;
    };

    prot Vector2Int prefabTilesGSize;

    prot Array2D<PrefabTile> prefabGrid_room_edge_wall;
    prot Array2D<PrefabTile> prefabGrid_room_edge_door;
    prot Array2D<PrefabTile> prefabGrid_room_corner;
    prot Array2D<PrefabTile> prefabGrid_room_center;
    prot Array2D<PrefabTile> prefabGrid_exitHallway;
    prot Array2D<PrefabTile> prefabGrid_empty;

    // Room 0 is default main room.
    // Room 1 is the exit hallway;
    prot RoomConfig roomConfigs[2]{
        RoomConfig{true/*canPlaceDoorsWithOtherRooms*/},
        RoomConfig{false/*canPlaceDoorsWithOtherRooms*/}
    };

    pub explicit RG_Prefabs(sp<CfgRoomGenerator> config, int seed = -1);

    prot void placePrefabInGrid(
        Vector2Int& placementPrefabsGPos,
        Array2D<PrefabTile>& prefabGrid,
        bool preFlipX, bool preFlipY, int postRotDeg);

    pub ~RG_Prefabs() override;
};
};
};
