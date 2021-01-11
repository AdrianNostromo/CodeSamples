#include "ZoneStartExtraData.h"

using namespace gridAdventure_a_v1;
using namespace gridAdventure_a_v1::GridBattleA;

ZoneStartExtraData::ZoneStartExtraData(
	std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
	std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId,
	int zoneLevelIndex)
	: super(teleportTargetEntityInstanceID, teleportTargetOffset,
		zeroHpZoneName, zeroHpZonePlayerPosEntityId),
	zoneLevelIndex(zoneLevelIndex)
{
	//void
}

ZoneStartExtraData::~ZoneStartExtraData() {
	//void
}
