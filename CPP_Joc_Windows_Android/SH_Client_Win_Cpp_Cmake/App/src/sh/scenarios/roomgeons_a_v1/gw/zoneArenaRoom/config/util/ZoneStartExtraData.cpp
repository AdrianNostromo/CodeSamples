#include "ZoneStartExtraData.h"

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::ArenaRoom;

ZoneStartExtraData::ZoneStartExtraData(
	int zoneInSequenceIndex,
	std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
	std::shared_ptr<std::string> _exitZoneName, std::shared_ptr<std::string> _exitZonePlayerPosEntityId)
	: super(teleportTargetEntityInstanceID, teleportTargetOffset),
	zoneInSequenceIndex(zoneInSequenceIndex),
	exitZoneName(_exitZoneName), exitZonePlayerPosEntityId(_exitZonePlayerPosEntityId)
{
	//void
}

ZoneStartExtraData::~ZoneStartExtraData() {
	//void
}
