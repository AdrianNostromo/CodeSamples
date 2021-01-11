#include "RG_RoomGrid.h"
#include <base/exceptions/LogicException.h>
#include "config/CfgRoomGenerator.h"

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

signed char RG_RoomGrid::Tile::SpecialRoomIds::OutsideOfRoom_WithWall = -1;
signed char RG_RoomGrid::Tile::SpecialRoomIds::OutsideOfRoom_NoWall = -2;

RG_RoomGrid::RG_RoomGrid(sp<CfgRoomGenerator> config, int seed)
	: super(config, seed)
{
	//void
}

void RG_RoomGrid::initTilesGrid(Vector2Int& prefabTilesGSize) {
	if (tilesGrid != nullptr) {
		throw LogicException(LOC);
	}

	tilesGridSize.set(roomPrefabsCount_withExitHallways.x * prefabTilesGSize.x, roomPrefabsCount_withExitHallways.y * prefabTilesGSize.y);
	int byteCount = tilesGridSize.x * tilesGridSize.y * sizeof(Tile);
	tilesGrid = static_cast<Tile*>(malloc(byteCount));
	memset(tilesGrid, 0, byteCount);

	floorTilingGrid = std::make_shared<Array2D<signed char>>(tilesGridSize.x, tilesGridSize.y);
	floorTilingGrid->memSetData(-1);

	wallsTilingGrid = std::make_shared<Array2D<signed char>>(tilesGridSize.x, tilesGridSize.y);
	wallsTilingGrid->memSetData(-1);
}

RG_RoomGrid::~RG_RoomGrid() {
	if (tilesGrid != nullptr) {
		free(tilesGrid);
		tilesGrid = nullptr;
	}
}
