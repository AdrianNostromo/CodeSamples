#include "ZoneStartExtraData.h"

using namespace randDungeons_a_v1;
using namespace randDungeons_a_v1::common;

ZoneStartExtraData::ZoneStartExtraData(
	std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
	std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId)
	: super(teleportTargetEntityInstanceID, teleportTargetOffset),
	zeroHpZoneName(zeroHpZoneName), zeroHpZonePlayerPosEntityId(zeroHpZonePlayerPosEntityId)
{
	//void
}

ZoneStartExtraData::~ZoneStartExtraData() {
	//void
}
