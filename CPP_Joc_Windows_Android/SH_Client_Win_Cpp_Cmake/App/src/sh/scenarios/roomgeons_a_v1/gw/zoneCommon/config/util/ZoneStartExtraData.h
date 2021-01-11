#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/contentZones/zone/util/ZoneStartExtraData.h>
#include <string>
#include <memory>

namespace roomgeons_a_v1 {
namespace common {
class ZoneStartExtraData : public worldGame3D::ZoneStartExtraData {priv typedef worldGame3D::ZoneStartExtraData super;pub dCtor(ZoneStartExtraData);
	pub explicit ZoneStartExtraData(
		std::shared_ptr<std::string> teleportTargetEntityInstanceID, sp<Vector3> teleportTargetOffset);

    pub ~ZoneStartExtraData() override;
};
};
};
