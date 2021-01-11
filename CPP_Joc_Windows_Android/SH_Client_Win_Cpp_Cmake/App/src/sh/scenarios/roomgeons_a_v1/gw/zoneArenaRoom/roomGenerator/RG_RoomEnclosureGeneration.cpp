#include "RG_RoomEnclosureGeneration.h"

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

RG_RoomEnclosureGeneration::RG_RoomEnclosureGeneration(sp<CfgRoomGenerator> config, int seed)
	: super(config, seed)
{
	//void
}

void RG_RoomEnclosureGeneration::generateWallsAndDoors() {
	for (int tilesGX = 0; tilesGX < tilesGridSize.x; tilesGX++) {
		for (int tilesGY = 0; tilesGY < tilesGridSize.y; tilesGY++) {
			Tile& tile = tilesGrid[tilesGY * tilesGridSize.x + tilesGX];

			if (tile.roomId < 0) {
				// This is a outside tile.
				// No wall needed.
				continue;
			}

			RoomConfig& roomConfig = roomConfigs[tile.roomId];

			Tile* xPrevTile = (tilesGX >= 1) ? &tilesGrid[tilesGY * tilesGridSize.x + (tilesGX - 1)] : nullptr;
			Tile* xNextTile = (tilesGX + 1) < tilesGridSize.x ? &tilesGrid[tilesGY * tilesGridSize.x + (tilesGX + 1)] : nullptr;

			Tile* yPrevTile = (tilesGY >= 1) ? &tilesGrid[(tilesGY - 1) * tilesGridSize.x + tilesGX] : nullptr;
			Tile* yNextTile = (tilesGY + 1 < tilesGridSize.x) ? &tilesGrid[(tilesGY + 1) * tilesGridSize.x + tilesGX] : nullptr;

			Tile* xPrev_yPrev_tile = (tilesGX >= 1 && tilesGY >= 1)									  ? &tilesGrid[(tilesGY - 1) * tilesGridSize.x + (tilesGX - 1)] : nullptr;
			Tile* xPrev_yNext_tile = (tilesGX >= 1 && tilesGY + 1 < tilesGridSize.x)				  ? &tilesGrid[(tilesGY + 1) * tilesGridSize.x + (tilesGX - 1)] : nullptr;
			Tile* xNext_yPrev_tile = (tilesGX + 1 < tilesGridSize.x && tilesGY >= 1)				  ? &tilesGrid[(tilesGY - 1) * tilesGridSize.x + (tilesGX + 1)] : nullptr;
			Tile* xNext_yNext_tile = (tilesGX + 1 < tilesGridSize.x && tilesGY + 1 < tilesGridSize.x) ? &tilesGrid[(tilesGY + 1) * tilesGridSize.x + (tilesGX + 1)] : nullptr;

			// First check if door is required because these thick tiled doors have side walls.
			if (roomConfig.canPlaceDoorsWithOtherRooms && (xPrevTile != nullptr && xPrevTile->roomId >= 0 && xPrevTile->roomId != tile.roomId)) {
				// XNeg door needed;
				// Try to extend a existing door.
				Door* door = yPrevTile != nullptr ? yPrevTile->sideDoorsArray[TileSideType::XNeg] : nullptr;
				if (door == nullptr) {
					// No existing door found, create a new one.
					Vector2Int tilesGPos{ tilesGX, tilesGY };
					door = &doorsList.appendEmplace(tilesGPos/*tilesGPos*/, TileSideType::XNeg/*tileSideType*/, 0/*tilesLength*/)->data;
				}

				door->tilesLength++;
				tile.sideDoorsArray[TileSideType::XNeg] = door;
			} else if (roomConfig.canPlaceDoorsWithOtherRooms && (xNextTile != nullptr && xNextTile->roomId >= 0 && xNextTile->roomId != tile.roomId)) {
				// XPos door needed;
				// Try to extend a existing door.
				Door* door = yPrevTile != nullptr ? yPrevTile->sideDoorsArray[TileSideType::XPos] : nullptr;
				if (door == nullptr) {
					// No existing door found, create a new one.
					Vector2Int tilesGPos{ tilesGX, tilesGY };
					door = &doorsList.appendEmplace(tilesGPos/*tilesGPos*/, TileSideType::XPos/*tileSideType*/, 0/*tilesLength*/)->data;
				}

				door->tilesLength++;
				tile.sideDoorsArray[TileSideType::XPos] = door;
			} else if (roomConfig.canPlaceDoorsWithOtherRooms && (yPrevTile != nullptr && yPrevTile->roomId >= 0 && yPrevTile->roomId != tile.roomId)) {
				// YNeg door needed;
				// Try to extend a existing door.
				Door* door = xPrevTile != nullptr ? xPrevTile->sideDoorsArray[TileSideType::YNeg] : nullptr;
				if (door == nullptr) {
					// No existing door found, create a new one.
					Vector2Int tilesGPos{ tilesGX, tilesGY };
					door = &doorsList.appendEmplace(tilesGPos/*tilesGPos*/, TileSideType::YNeg/*tileSideType*/, 0/*tilesLength*/)->data;
				}

				door->tilesLength++;
				tile.sideDoorsArray[TileSideType::YNeg] = door;
			} else if (roomConfig.canPlaceDoorsWithOtherRooms && (yNextTile != nullptr && yNextTile->roomId >= 0 && yNextTile->roomId != tile.roomId)) {
				// YPos door needed;
				// Try to extend a existing door.
				Door* door = xPrevTile != nullptr ? xPrevTile->sideDoorsArray[TileSideType::YPos] : nullptr;
				if (door == nullptr) {
					// No existing door found, create a new one.
					Vector2Int tilesGPos{ tilesGX, tilesGY };
					door = &doorsList.appendEmplace(tilesGPos/*tilesGPos*/, TileSideType::YPos/*tileSideType*/, 0/*tilesLength*/)->data;
				}

				door->tilesLength++;
				tile.sideDoorsArray[TileSideType::YPos] = door;
			} else if (
				xPrevTile == nullptr/*grid_edge_prev*/ || xPrevTile->roomId == Tile::SpecialRoomIds::OutsideOfRoom_WithWall/*out_of_room_withWall*/ ||
				xNextTile == nullptr/*grid_edge_prev*/ || xNextTile->roomId == Tile::SpecialRoomIds::OutsideOfRoom_WithWall/*out_of_room_withWall*/ ||
				yPrevTile == nullptr/*grid_edge_prev*/ || yPrevTile->roomId == Tile::SpecialRoomIds::OutsideOfRoom_WithWall/*out_of_room_withWall*/ ||
				yNextTile == nullptr/*grid_edge_prev*/ || yNextTile->roomId == Tile::SpecialRoomIds::OutsideOfRoom_WithWall/*out_of_room_withWall*/ ||

				xPrev_yPrev_tile == nullptr/*grid_edge_prev*/ || xPrev_yPrev_tile->roomId == Tile::SpecialRoomIds::OutsideOfRoom_WithWall/*out_of_room_withWall*/ ||
				xPrev_yNext_tile == nullptr/*grid_edge_prev*/ || xPrev_yNext_tile->roomId == Tile::SpecialRoomIds::OutsideOfRoom_WithWall/*out_of_room_withWall*/ ||
				xNext_yPrev_tile == nullptr/*grid_edge_prev*/ || xNext_yPrev_tile->roomId == Tile::SpecialRoomIds::OutsideOfRoom_WithWall/*out_of_room_withWall*/ ||
				xNext_yNext_tile == nullptr/*grid_edge_prev*/ || xNext_yNext_tile->roomId == Tile::SpecialRoomIds::OutsideOfRoom_WithWall/*out_of_room_withWall*/)
			{
				// Using the current logic of tiling walls, some places where different room connect need to be specially crafted so a door will fit in the location (2 tiles are reserved for walls in most cases).
				wallsTilingGrid->setDirect(tilesGX, tilesGY, (tilesGX + tilesGY) % 2);
			}
		}
	}
}

RG_RoomEnclosureGeneration::~RG_RoomEnclosureGeneration() {
	//void
}
