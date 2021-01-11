#include "REP_Floors.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

REP_Floors::REP_Floors()
	: super()
{
	//void
}

void REP_Floors::createFloorTilingGrid() {
	result->floorIndexedTilingGrid.resizeNoElementsMove(
		roomZoneSoftBounds.getLenX() * gridCellTilesSize,
		roomZoneSoftBounds.getLenY() * gridCellTilesSize
	);
	result->floorIndexedTilingGrid.memSetData(-1);

	// Enum all gCells in the room softBounds and set floorIndexedTilingGrid values to -1 or 0.
	for (int zoneGX = roomZoneSoftBounds.min.x; zoneGX < roomZoneSoftBounds.max.x; zoneGX++) {
		for (int zoneGY = roomZoneSoftBounds.min.y; zoneGY < roomZoneSoftBounds.max.y; zoneGY++) {
			ZoneGridCell& cGCell = zoneGrid->getReference(zoneGX, zoneGY);

			if (cGCell.roomIndex != roomIndex) {
				continue;
			}

			Vector2Int roomGPos{
				(zoneGX - roomZoneSoftBounds.min.x) * gridCellTilesSize,
				(zoneGY - roomZoneSoftBounds.min.y) * gridCellTilesSize
			};
			
			for (int roomGOffsetX = 0; roomGOffsetX < gridCellTilesSize; roomGOffsetX++) {
				for (int roomGOffsetY = 0; roomGOffsetY < gridCellTilesSize; roomGOffsetY++) {
					if (cGCell.roomIndex == roomIndex) {
						result->floorIndexedTilingGrid.setDirect(roomGPos.x + roomGOffsetX, roomGPos.y + roomGOffsetY, 0);
					} else {
						result->floorIndexedTilingGrid.setDirect(roomGPos.x + roomGOffsetX, roomGPos.y + roomGOffsetY, -1);
					}

					oLevelZoneRoomIndexesGrid->setDirect(
						zoneGX * gridCellTilesSize + roomGOffsetX,
						zoneGY * gridCellTilesSize + roomGOffsetY,
						roomIndex
					);
				}
			}
		}
	}
}

REP_Floors::~REP_Floors() {
	//void
}
