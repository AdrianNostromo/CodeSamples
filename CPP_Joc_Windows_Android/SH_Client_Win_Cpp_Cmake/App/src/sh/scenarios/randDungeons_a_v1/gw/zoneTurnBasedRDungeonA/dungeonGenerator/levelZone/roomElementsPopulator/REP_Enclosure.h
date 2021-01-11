#pragma once

#include <base/gh.h>
#include "REP_Floors.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class REP_Enclosure : public REP_Floors {priv typedef REP_Floors super;pub dCtor(REP_Enclosure);
    pub explicit REP_Enclosure();

	prot void prepareSideEementsInfoGrid();
	priv void prepareSideEementsInfoGridCellSide(
		ZoneGridCell& cGCell, int zoneGridX, int zoneGridY,
		int sideIndex, Vector2Int& sideNeighbourOffset,
		int roadDirNeededForDoor);

	priv void setSideElementsWallCells(
		Bounds2DInt& sideElementsBounds,
		int sideIndex);
	priv void setSideElementsDoorCells(
		int tilesGX, int tilesGY,
		int sideIndex, int tilesCount,
		int doorOtherSideRoomIndex);
	prot void combineSideElementsAndCreateList();
	priv void processTilesGridSideElementCombining(
		SideEsementsInfo& sideEsementsInfo, int tilesGX, int tilesGY, int sideIndex,
		Vector2Int& neighbourOffset);

    pub ~REP_Enclosure() override;
};
};
};
