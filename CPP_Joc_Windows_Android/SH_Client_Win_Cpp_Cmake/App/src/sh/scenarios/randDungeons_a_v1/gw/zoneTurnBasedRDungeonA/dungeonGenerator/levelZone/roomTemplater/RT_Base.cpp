#include "RT_Base.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::TurnBasedRDungeonA;

RT_Base::RT_Base()
	: super()
{
	//void
}

void RT_Base::generate(
	std::shared_ptr<CfgDungeonGenerator>& config, Vector3& roomOriginZonePos, std::shared_ptr<Result_RoomElementsPopulator> in_roomElementsPopulatorResult,
	int zoneIndex)
{
	this->config = config;
	this->in_roomElementsPopulatorResult = in_roomElementsPopulatorResult;
	this->zoneIndex = zoneIndex;

	result = std::make_shared<Result_RoomTemplater>();
	result->roomContent = std::make_shared<DungeonContent::LevelZone::Room>(roomOriginZonePos);
}

RT_Base::~RT_Base() {
	//void
}
