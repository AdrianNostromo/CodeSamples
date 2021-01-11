#include "ZoneStartExtraData.h"

using namespace worldGame3D;

ZoneStartExtraData::ZoneStartExtraData(std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset)
	: teleportTargetEntityInstanceID(teleportTargetEntityInstanceID), teleportTargetOffset(teleportTargetOffset)
{
	//void
}

ZoneStartExtraData::~ZoneStartExtraData() {
	//void
}
