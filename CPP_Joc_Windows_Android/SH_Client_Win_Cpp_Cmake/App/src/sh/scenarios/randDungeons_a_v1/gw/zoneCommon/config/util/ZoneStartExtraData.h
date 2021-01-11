#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/contentZones/zone/util/ZoneStartExtraData.h>
#include <string>
#include <memory>

namespace randDungeons_a_v1 {
namespace common {
class ZoneStartExtraData : public worldGame3D::ZoneStartExtraData {priv typedef worldGame3D::ZoneStartExtraData super;pub dCtor(ZoneStartExtraData);
	pub std::shared_ptr<std::string> zeroHpZoneName;
	pub std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId;

	pub explicit ZoneStartExtraData(
		std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset,
		std::shared_ptr<std::string> zeroHpZoneName, std::shared_ptr<std::string> zeroHpZonePlayerPosEntityId);

    pub ~ZoneStartExtraData() override;
};
};
};
