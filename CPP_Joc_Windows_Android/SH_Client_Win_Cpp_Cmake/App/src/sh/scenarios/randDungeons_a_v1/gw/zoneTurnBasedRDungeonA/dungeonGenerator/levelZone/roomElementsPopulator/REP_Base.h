#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <memory>
#include "util/Result_RoomElementsPopulator.h"
#include <base/math/Vector2Int.h>
#include <base/list/Array2D.h>
#include <base/list/ArrayList.h>
#include "../util/ZoneGridCell.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class REP_Base : public base::Object {priv typedef base::Object super;pub dCtor(REP_Base);
	prot class SideEsementsInfo {
		pub class SideElement {
			pub Result_RoomElementsPopulator::SideElementType type = Result_RoomElementsPopulator::SideElementType::None;
			pub int tilesLen = 0;
			pub int doorOtherSideRoomIndex = -1;
		};

		// The indexes are from RoomGCellSides;
		pub SideElement sidesList[4];
	};

	prot int roomIndex = -1;
	prot int gridCellTilesSize = -1;
	prot int doorsVisualTilesSize = -1;
	prot Bounds2DInt roomZoneSoftBounds{};
	prot Array2D<ZoneGridCell>* zoneGrid = nullptr;

	prot bool hasEntryPointStairs = false;
	prot bool hasExitStairs = false;
	prot bool hasStairsUp = false;
	prot bool hasStairsDown = false;
	prot int mobsCount = 0;
	prot std::shared_ptr<Array2D<short>> oLevelZoneRoomIndexesGrid;

	pub std::shared_ptr<Result_RoomElementsPopulator> result = nullptr;
    
	prot std::unique_ptr<Array2D<SideEsementsInfo>> roomTilesInfoGrid = nullptr;

    pub explicit REP_Base();

    pub virtual void generate(
		int roomIndex,
		int gridCellTilesSize, int doorsVisualTilesSize,
		Bounds2DInt& roomZoneSoftBounds,
		Array2D<ZoneGridCell>* zoneGrid,
		bool hasEntryPointStairs, bool hasExitStairs, bool hasStairsUp, bool hasStairsDown,
		int mobsCount,
		std::shared_ptr<Array2D<short>>& oLevelZoneRoomIndexesGrid);

    prot void logTilesGrid();

    pub ~REP_Base() override;
};
};
};
