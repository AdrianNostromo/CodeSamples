#pragma once

#include <base/gh.h>
#include "../../../zoneCommon/config/util/ZoneStartExtraData.h"
#include "../../../config/entity/EntityIds.h"
#include <memory>
#include <base/math/Vector2Int.h>

namespace roomgeons_a_v1 {
namespace ArenaRoom {
class ZoneStartExtraData : public common::ZoneStartExtraData {priv typedef common::ZoneStartExtraData super;pub dCtor(ZoneStartExtraData);
	pub int zoneInSequenceIndex;

	pub std::shared_ptr<std::string> exitZoneName;
	pub std::shared_ptr<std::string> exitZonePlayerPosEntityId;

    pub explicit ZoneStartExtraData(
		int zoneInSequenceIndex,
		std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
		std::shared_ptr<std::string> exitZoneName, std::shared_ptr<std::string> exitZonePlayerPosEntityId);

    pub ~ZoneStartExtraData() override;
};
};
};
