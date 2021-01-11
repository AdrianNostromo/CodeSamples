#pragma once

#include <base/gh.h>
#include "RG_Base.h"
#include <base/list/Array2D.h>
#include <memory>

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class RG_RoomGrid : public RG_Base {priv typedef RG_Base super;pub dCtor(RG_RoomGrid);
    pub enum TileSideType { YNeg=0, XPos=1, YPos=2, XNeg=3};

    prot class Door {pub dCtor(Door);
        pub Vector2Int tilesGPos;
        // 0: x_positive, 1: y_pos, 2: x_neg, 3: y_neg;
        pub TileSideType tileSideType;
        // Walls are always grown to xPos or yPos always.
        pub int tilesLength;

        pub explicit Door(Vector2Int& tilesGPos, TileSideType tileSideType, int tilesLength)
            : tilesGPos(tilesGPos), tileSideType(tileSideType), tilesLength(tilesLength)
        {
            //void
        }
    };
    prot class Tile {
        pub class SpecialRoomIds {
            pub static signed char OutsideOfRoom_WithWall;// = -1;
            pub static signed char OutsideOfRoom_NoWall;// = -2;
        };
        // -1 : outside of room (with wall)
        // -2 : outside of room (no wall)
        // >=0 : room
        pub signed char roomId;

        pub Door* sideDoorsArray[4];
    };

    prot Vector2Int tilesGridSize{};
    prot Tile* tilesGrid = nullptr;

    std::shared_ptr<Array2D<signed char>> floorTilingGrid = nullptr;
    // This is for the tiling walls that tile in 2d.
    std::shared_ptr<Array2D<signed char>> wallsTilingGrid = nullptr;

    pub explicit RG_RoomGrid(sp<CfgRoomGenerator> config, int seed = -1);

    prot void initTilesGrid(Vector2Int& prefabTilesGSize);

    pub ~RG_RoomGrid() override;
};
};
};
