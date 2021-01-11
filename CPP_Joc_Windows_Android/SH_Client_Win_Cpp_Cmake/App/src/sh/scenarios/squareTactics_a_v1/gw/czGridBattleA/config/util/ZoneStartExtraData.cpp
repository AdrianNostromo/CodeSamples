#include "ZoneStartExtraData.h"

using namespace squareTactics_a_v1;
using namespace squareTactics_a_v1::czGridBattleA;

ZoneStartExtraData::ZoneStartExtraData(
	std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
	std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId)
	: super(
		teleportTargetEntityInstanceID, teleportTargetOffset,
		zeroHpZoneName, zeroHpZonePlayerPosEntityId)
{
	//void
}

ZoneStartExtraData::~ZoneStartExtraData() {
	//void
}
