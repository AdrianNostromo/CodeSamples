#include "RoomTemplater.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

RoomTemplater::RoomTemplater()
	: super()
{
	//void
}

void RoomTemplater::generate(
	std::shared_ptr<CfgDungeonGenerator>& config, Vector3& roomOriginZonePos, std::shared_ptr<Result_RoomElementsPopulator> in_roomElementsPopulatorResult,
	int zoneIndex)
{
	super::generate(
		config, roomOriginZonePos, in_roomElementsPopulatorResult,
		zoneIndex
	);

	bakeFloor();
	bakeEnclosure();
	bakeTeleporters();
	bakeMobs();
}

RoomTemplater::~RoomTemplater() {
	reservedDisposeIfNeeded();// Note1001
}
