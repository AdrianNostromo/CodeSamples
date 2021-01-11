#pragma once

#include <base/gh.h>
#include <sh/scenarios/gridAdventure_a_v1/gw/zoneCommon/config/util/ZoneStartExtraData.h>
#include <string>
#include <memory>

namespace gridAdventure_a_v1 {
namespace GridBattleA {
class ZoneStartExtraData : public common::ZoneStartExtraData {priv typedef common::ZoneStartExtraData super;pub dCtor(ZoneStartExtraData);
	pub int zoneLevelIndex;

	pub explicit ZoneStartExtraData(
		std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
		std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId,
		int zoneLevelIndex);

    pub ~ZoneStartExtraData() override;
};
};
};
