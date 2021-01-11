#pragma once

#include <base/gh.h>
#include "LZG_Roads.h"
#include <base/math/PosLen2DInt.h>
#include <base/math/Bounds2DInt.h>
#include "roomGrower/RoomGrower.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class LZG_GridRomify : public LZG_Roads {priv typedef LZG_Roads super;pub dCtor(LZG_GridRomify);
	prot class RoomType {
		pub static const int IN;
		pub static const int OUT;
		pub static const int SPECIAL;
		pub static const int FILLER;
	};
	priv struct RoomGrowSolution {
		pub bool x_canGrow = false;
		pub int xGrow_roadGCellsEngulfedCount = 0;

		pub bool invX_canGrow = false;
		pub int invXGrow_roadGCellsEngulfedCount = 0;

		pub bool y_canGrow = false;
		pub int yGrow_roadGCellsEngulfedCount = 0;

		pub bool invY_canGrow = false;
		pub int invYGrow_roadGCellsEngulfedCount = 0;
	};

	prot ArrayList<Bounds2DInt> roomifiedGridRoomZoneBoundsList{};
	
	pub explicit LZG_GridRomify(
        std::shared_ptr<CfgDungeonGenerator>& config, int zoneIndex,
		Random* rng);

	prot void roomifyGrid();
	priv void autoGrowRoom(
		int roomIndex, Vector2Int& startPos, CfgDungeonGenerator::RoomSizing* roomSizing);

    pub ~LZG_GridRomify() override;
};
};
};
