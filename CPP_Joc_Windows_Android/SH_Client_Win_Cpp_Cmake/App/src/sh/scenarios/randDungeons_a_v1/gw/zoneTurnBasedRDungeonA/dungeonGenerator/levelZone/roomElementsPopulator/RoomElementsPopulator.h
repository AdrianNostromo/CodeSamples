#pragma once

#include <base/gh.h>
#include "REP_Mobs.h"

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class RoomElementsPopulator final/*Note1001. calls_the_reserved_dispose*/ : public REP_Mobs {priv typedef REP_Mobs super;pub dCtor(RoomElementsPopulator);
    pub explicit RoomElementsPopulator();

	pub void generate(
		int roomIndex,
		int gridCellTilesSize, int doorsVisualTilesSize,
		Bounds2DInt& roomZoneSoftBounds,
		Array2D<ZoneGridCell>* zoneGrid,
		bool hasEntryPointStairs, bool hasExitStairs, bool hasStairsUp, bool hasStairsDown,
		int mobsCount,
		std::shared_ptr<Array2D<short>>& oLevelZoneRoomIndexesGrid) override;

    pub ~RoomElementsPopulator() override;
};
};
};
