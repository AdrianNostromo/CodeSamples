#include "REP_Teleporters.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::RDungeonA;

REP_Teleporters::REP_Teleporters()
	: super()
{
	//void
}

void REP_Teleporters::placeTeleporters() {
	if (hasEntryPointStairs) {
		// This room is empty, use the center pos for this teleport.
		Vector2Int v{
			(int)Math::floor((roomZoneSoftBounds.getLenX() * gridCellTilesSize) / 2),
			(int)Math::floor((roomZoneSoftBounds.getLenY() * gridCellTilesSize) / 2)
		};
		result->teleportersList.append_emplace( v, Result_RoomElementsPopulator::TeleporterInfo::Type::EntryPointStairs);
	}

	if (hasExitStairs) {
		Vector2Int v{1, 0};
		result->teleportersList.append_emplace( v, Result_RoomElementsPopulator::TeleporterInfo::Type::ExitStairs);
	}

	if (hasStairsUp) {
		// This room is empty, use the center pos for this teleport.
		Vector2Int v{
			(int)Math::floor((roomZoneSoftBounds.getLenX() * gridCellTilesSize) / 2),
			(int)Math::floor((roomZoneSoftBounds.getLenY() * gridCellTilesSize) / 2)
		};
		result->teleportersList.append_emplace(v, Result_RoomElementsPopulator::TeleporterInfo::Type::StairsUp);
	}

	if (hasStairsDown) {
		Vector2Int v{ 3, 0 };
		result->teleportersList.append_emplace(v, Result_RoomElementsPopulator::TeleporterInfo::Type::StairsDown);
	}
}

REP_Teleporters::~REP_Teleporters() {
	//void
}
