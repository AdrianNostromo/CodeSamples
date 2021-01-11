#pragma once

#include <base/gh.h>
#include "../../../czCommon/config/util/ZoneStartExtraData.h"
#include <string>
#include <memory>
#include <base/list/ListDL.h>
#include <base/sp.h>

namespace base {
	class WrappedWorldEntity;
};

namespace squareTactics_a_v1 {
namespace czGridBattleA {
class ZoneStartExtraData : public czCommon::ZoneStartExtraData {priv typedef czCommon::ZoneStartExtraData super;pub dCtor(ZoneStartExtraData);
	// Note. Convert to a constructor param initialisator if needed.
	pub ListDL<sp<base::WrappedWorldEntity>> partyEntities{};

	pub explicit ZoneStartExtraData(
		std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
		std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId);

    pub ~ZoneStartExtraData() override;
};
};
};
