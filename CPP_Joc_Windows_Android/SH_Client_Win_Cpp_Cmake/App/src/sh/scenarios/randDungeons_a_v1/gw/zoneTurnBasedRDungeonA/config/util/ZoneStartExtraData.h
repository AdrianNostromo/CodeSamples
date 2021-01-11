#pragma once

#include <base/gh.h>
#include "../../../zoneCommon/config/util/ZoneStartExtraData.h"
#include <memory>

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
	class DungeonContent;

class ZoneStartExtraData : public common::ZoneStartExtraData {priv typedef common::ZoneStartExtraData super;pub dCtor(ZoneStartExtraData);
	pub std::shared_ptr< TurnBasedRDungeonA::DungeonContent> dungeonContent;

	pub int levelZoneToActivate;

	pub std::shared_ptr<std::string> exitZoneName;
	pub std::shared_ptr<std::string> exitZonePlayerPosEntityId;

    pub explicit ZoneStartExtraData(
		std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
		std::shared_ptr< TurnBasedRDungeonA::DungeonContent>& dungeonContent, int levelZoneToActivate,
		std::shared_ptr<std::string> exitZoneName, std::shared_ptr<std::string> exitZonePlayerPosEntityId,
		std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId);

    pub ~ZoneStartExtraData() override;
};
};
};