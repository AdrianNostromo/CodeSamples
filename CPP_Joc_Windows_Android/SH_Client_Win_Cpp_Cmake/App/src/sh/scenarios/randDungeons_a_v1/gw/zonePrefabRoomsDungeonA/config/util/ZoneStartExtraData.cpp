#include "ZoneStartExtraData.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::PrefabRoomsDungeonA;

ZoneStartExtraData::ZoneStartExtraData(
	std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
	std::shared_ptr< DungeonContent>& dungeonContent, int levelZoneToActivate,
	std::shared_ptr<std::string> exitZoneName, std::shared_ptr<std::string> exitZonePlayerPosEntityId,
	std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId)
	: super(teleportTargetEntityInstanceID, teleportTargetOffset,
		zeroHpZoneName, zeroHpZonePlayerPosEntityId),
	dungeonContent(dungeonContent), levelZoneToActivate(levelZoneToActivate),
	exitZoneName(exitZoneName), exitZonePlayerPosEntityId(exitZonePlayerPosEntityId)
{
	//void
}

ZoneStartExtraData::~ZoneStartExtraData() {
	//void
}
