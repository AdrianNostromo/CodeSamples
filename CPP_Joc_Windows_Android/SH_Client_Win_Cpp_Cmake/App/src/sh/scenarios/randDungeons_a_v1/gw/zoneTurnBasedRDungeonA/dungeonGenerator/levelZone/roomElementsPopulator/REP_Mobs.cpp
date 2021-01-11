#include "REP_Mobs.h"
#include <base/math/Math.h>

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

REP_Mobs::REP_Mobs()
	: super()
{
	//void
}

void REP_Mobs::placeMobsGroup() {
	if (mobsCount <= 0) {
		return;
	}

	Vector2Int roomCenterGPos{
		(int)Math::floor((roomZoneSoftBounds.getLenX() * gridCellTilesSize) / 2),
		(int)Math::floor((roomZoneSoftBounds.getLenY() * gridCellTilesSize) / 2)
	};
	// Get a center pos and use a ulam spiral to get the pos for each mob.
	for (int i = 0; i < mobsCount; i++) {
		Vector2Int offsetGPos = Math::UlamSpiral::ValueToPositionStartsAt1(1 + i);

		Vector2Int finalGPos{
			roomCenterGPos.x + offsetGPos.x,
			roomCenterGPos.y + offsetGPos.y
		};

		result->mobsList.append_emplace(finalGPos, Result_RoomElementsPopulator::MobInfo::Type::GeneralMob);
	}
}

REP_Mobs::~REP_Mobs() {
	//void
}
