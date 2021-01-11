#include "REP_Enclosure.h"
#include <iostream>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

REP_Enclosure::REP_Enclosure()
	: super()
{
	//void
}

void REP_Enclosure::prepareSideEementsInfoGrid() {
	struct t1 { int sideIndex; Vector2Int sideNeighbourOffset; int roadDir; };
	t1 sidesInfoList[4]{
		{Result_RoomElementsPopulator::RoomGCellSides::xNegative/*left wall*/, Vector2Int{-1, 0}/*sideNeighbourOffset*/, ZoneGridCell::RoadDir::xNegative/*roadDir*/},
		{Result_RoomElementsPopulator::RoomGCellSides::xPositive/*right wall*/, Vector2Int{1, 0}/*sideNeighbourOffset*/, ZoneGridCell::RoadDir::xPositive/*roadDir*/},
		{Result_RoomElementsPopulator::RoomGCellSides::yNegative/*bottom wall*/, Vector2Int{0, -1}/*sideNeighbourOffset*/, ZoneGridCell::RoadDir::yNegative/*roadDir*/},
		{Result_RoomElementsPopulator::RoomGCellSides::yPositive/*top wall*/, Vector2Int{0, 1}/*sideNeighbourOffset*/, ZoneGridCell::RoadDir::yPositive/*roadDir*/}
	};

	// Enum all zoneGrid gCells in the bounds area to fill roomTilesInfoGrid with walls on the sides.
	// Also set the doors where needed.
	for (int gX = roomZoneSoftBounds.min.x; gX < roomZoneSoftBounds.max.x; gX++) {
		for (int gY = roomZoneSoftBounds.min.y; gY < roomZoneSoftBounds.max.y; gY++) {
			ZoneGridCell& cGCell = zoneGrid->getReference(gX, gY);

			if (cGCell.roomIndex == roomIndex) {
				// Walls are created on bounds_edge and on edges if the neighbour gCell is not the same roomIndex.
				// Process each side individually.

				for (int i = 0; i < 4; i++) {
					t1& sidesInfo = sidesInfoList[i];

					prepareSideEementsInfoGridCellSide(
						cGCell, gX, gY,
						sidesInfo.sideIndex, sidesInfo.sideNeighbourOffset,
						sidesInfo.roadDir
					);
				}
			}
		}
	}
}

void REP_Enclosure::prepareSideEementsInfoGridCellSide(
	ZoneGridCell& cGCell, int zoneGridX, int zoneGridY,
	int sideIndex, Vector2Int& sideNeighbourOffset,
	int roadDirNeededForDoor)
{
	Vector2Int neighbourZoneGPos{ zoneGridX + sideNeighbourOffset.x, zoneGridY + sideNeighbourOffset.y };
	ZoneGridCell* neighbourZoneGCell = zoneGrid->getPointer(neighbourZoneGPos.x, neighbourZoneGPos.y, nullptr);

	if (neighbourZoneGCell == nullptr/*outside_of_room_bounds*/ || neighbourZoneGCell->roomIndex != cGCell.roomIndex/*different_room*/) {
		// Create a side_element in this direction.
		// Check if a door or wall is needed.
		if ((cGCell.roadDirBitMap & roadDirNeededForDoor) != 0) {
			// Door;
			if (neighbourZoneGCell == nullptr/*outside_of_room_bounds*/) {
				// Doors must lead to other rooms, no to void.
				throw LogicException(LOC);
			}

			int tilesCount = doorsVisualTilesSize;
			Vector2Int finalTileGPos{ (zoneGridX - roomZoneSoftBounds.min.x) * gridCellTilesSize, (zoneGridY - roomZoneSoftBounds.min.y) * gridCellTilesSize };
			if (sideNeighbourOffset.x == -1) {
				// Left wall.
				finalTileGPos.y += (tilesCount - 1);
			} else if (sideNeighbourOffset.x == 1) {
				// Right wall.
				finalTileGPos.x += (gridCellTilesSize - 1);
			} else if (sideNeighbourOffset.y == -1) {
				// Bottom wall.
				//void
			} else if (sideNeighbourOffset.y == 1) {
				// Top wall.
				finalTileGPos.x += (tilesCount - 1);
				finalTileGPos.y += (gridCellTilesSize - 1);
			} else {
				throw LogicException(LOC);
			}
			setSideElementsDoorCells(
				finalTileGPos.x, finalTileGPos.y,
				sideIndex, tilesCount,
				neighbourZoneGCell->roomIndex
			);
		} else {
			// Wall;
			Bounds2DInt sideElementsBounds{};
			sideElementsBounds.min.set((zoneGridX - roomZoneSoftBounds.min.x) * gridCellTilesSize, (zoneGridY - roomZoneSoftBounds.min.y) * gridCellTilesSize);
			sideElementsBounds.max.set(sideElementsBounds.min.x + gridCellTilesSize, sideElementsBounds.min.y + gridCellTilesSize);

			// Based on the side type, keep only a strip from sideElementsBounds;
			if (sideNeighbourOffset.x == -1) {
				// Left wall.
				sideElementsBounds.max.x = sideElementsBounds.min.x + 1;
			} else if (sideNeighbourOffset.x == 1) {
				// Right wall.
				sideElementsBounds.min.x = sideElementsBounds.max.x - 1;
			} else if (sideNeighbourOffset.y == -1) {
				// Bottom wall.
				sideElementsBounds.max.y = sideElementsBounds.min.y + 1;
			} else if (sideNeighbourOffset.y == 1) {
				// Top wall.
				sideElementsBounds.min.y = sideElementsBounds.max.y - 1;
			} else {
				throw LogicException(LOC);
			}

			setSideElementsWallCells(
				sideElementsBounds,
				sideIndex
			);
		}
	}
}

void REP_Enclosure::setSideElementsWallCells(
	Bounds2DInt& sideElementsBounds,
	int sideIndex)
{
	for (int tilesGX = sideElementsBounds.min.x; tilesGX < sideElementsBounds.max.x; tilesGX++) {
		for (int tilesGY = sideElementsBounds.min.y; tilesGY < sideElementsBounds.max.y; tilesGY++) {
			SideEsementsInfo& sideEsementsInfo = roomTilesInfoGrid->getReference(tilesGX, tilesGY);

			sideEsementsInfo.sidesList[sideIndex].type = Result_RoomElementsPopulator::SideElementType::Wall;
			sideEsementsInfo.sidesList[sideIndex].tilesLen = 1;
		}
	}
}

void REP_Enclosure::setSideElementsDoorCells(
	int tilesGX, int tilesGY,
	int sideIndex, int tilesCount,
	int doorOtherSideRoomIndex)
{
	SideEsementsInfo& sideEsementsInfo = roomTilesInfoGrid->getReference(tilesGX, tilesGY);

	sideEsementsInfo.sidesList[sideIndex].type = Result_RoomElementsPopulator::SideElementType::Door;
	sideEsementsInfo.sidesList[sideIndex].tilesLen = tilesCount;
	sideEsementsInfo.sidesList[sideIndex].doorOtherSideRoomIndex = doorOtherSideRoomIndex;
}

void REP_Enclosure::combineSideElementsAndCreateList() {
	// Walls are room inward oriented in a circle direction.
	// Do 2 enumerations of roomTilesInfoGrid to combine walls into a list of fewer walls but with a longer length.
	// Also creates the list of needed walls.
	Vector2Int neighbourOffset{};

	for (int tilesGX = 0; tilesGX < roomTilesInfoGrid->getLengthA(); tilesGX++) {
		for (int tilesGY = 0; tilesGY < roomTilesInfoGrid->getLengthB(); tilesGY++) {
			SideEsementsInfo& sideEsementsInfo = roomTilesInfoGrid->getReference(tilesGX, tilesGY);

			// Check 2 axes individually.
			// Try to move the top wall tile to the right.
			neighbourOffset.set(1, 0);
			processTilesGridSideElementCombining(sideEsementsInfo, tilesGX, tilesGY, Result_RoomElementsPopulator::RoomGCellSides::yPositive, neighbourOffset);
			// Try to move the left wall up.
			neighbourOffset.set(0, 1);
			processTilesGridSideElementCombining(sideEsementsInfo, tilesGX, tilesGY, Result_RoomElementsPopulator::RoomGCellSides::xNegative, neighbourOffset);
		}
	}
	for (int tilesGX = roomTilesInfoGrid->getLengthA() - 1; tilesGX >= 0; tilesGX--) {
		for (int tilesGY = roomTilesInfoGrid->getLengthB() - 1; tilesGY >= 0; tilesGY--) {
			SideEsementsInfo& sideEsementsInfo = roomTilesInfoGrid->getReference(tilesGX, tilesGY);

			// Check 2 axes individually.
			// Try to move the bottom wall tile to the left.
			neighbourOffset.set(-1, 0);
			processTilesGridSideElementCombining(sideEsementsInfo, tilesGX, tilesGY, Result_RoomElementsPopulator::RoomGCellSides::yNegative, neighbourOffset);
			// Try to move the right wall tile down.
			neighbourOffset.set(0, -1);
			processTilesGridSideElementCombining(sideEsementsInfo, tilesGX, tilesGY, Result_RoomElementsPopulator::RoomGCellSides::xPositive, neighbourOffset);
		}
	}
}

void REP_Enclosure::processTilesGridSideElementCombining(
	SideEsementsInfo& sideEsementsInfo, int tilesGX, int tilesGY, int sideIndex,
	Vector2Int& neighbourOffset)
{
	// Doors are not combined or tiled. They are already set to the correct len. Just save them in the elements list.
	if (sideEsementsInfo.sidesList[sideIndex].type != 0) {
		Vector2Int neighbourTilesGPos{ tilesGX + neighbourOffset.x, tilesGY + neighbourOffset.y };
		SideEsementsInfo* neighbourSideEsementsInfo = (neighbourTilesGPos.x >= 0 && neighbourTilesGPos.x < roomTilesInfoGrid->getLengthA() && neighbourTilesGPos.y >= 0 && neighbourTilesGPos.y < roomTilesInfoGrid->getLengthB()) ? roomTilesInfoGrid->getPointer(neighbourTilesGPos.x, neighbourTilesGPos.y) : nullptr;

		if (sideEsementsInfo.sidesList[sideIndex].type != Result_RoomElementsPopulator::SideElementType::Door &&
			(neighbourSideEsementsInfo != nullptr && sideEsementsInfo.sidesList[sideIndex].type == neighbourSideEsementsInfo->sidesList[sideIndex].type))
		{
			neighbourSideEsementsInfo->sidesList[sideIndex].tilesLen += sideEsementsInfo.sidesList[sideIndex].tilesLen;

			sideEsementsInfo.sidesList[sideIndex].type = Result_RoomElementsPopulator::SideElementType::None;
			sideEsementsInfo.sidesList[sideIndex].tilesLen = 0;
		} else {
			// Door (not combinable) or wall origin reached, save it in a list.
			Vector2Int v1{ tilesGX, tilesGY };
			result->finalSideElementOrigins.append_emplace(
				v1, sideIndex,
				sideEsementsInfo.sidesList[sideIndex].type,
				sideEsementsInfo.sidesList[sideIndex].tilesLen,
				sideEsementsInfo.sidesList[sideIndex].doorOtherSideRoomIndex
			);
		}
	}
}

REP_Enclosure::~REP_Enclosure() {
	//void
}
