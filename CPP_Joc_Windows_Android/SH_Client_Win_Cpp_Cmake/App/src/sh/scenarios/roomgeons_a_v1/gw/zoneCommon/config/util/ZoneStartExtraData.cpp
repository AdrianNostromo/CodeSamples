#include "ZoneStartExtraData.h"

using namespace roomgeons_a_v1;
using namespace roomgeons_a_v1::common;

ZoneStartExtraData::ZoneStartExtraData(
	std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset)
	: super(teleportTargetEntityInstanceID, teleportTargetOffset)
{
	//void
}

ZoneStartExtraData::~ZoneStartExtraData() {
	//void
}
