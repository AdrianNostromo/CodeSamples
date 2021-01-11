#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>
#include <memory>
#include <string>

namespace worldGame3D {
	class IContentZone;
	class ZoneStartExtraData;
	class ContentZone;
};

namespace worldGame3D {
class IGamePartContentZones : virtual public IGamePart {priv typedef IGamePart super;
	pub static int TYPE;
    DEC_componentBasics();

	pub virtual std::shared_ptr<ContentZone> getActiveContentZone() = 0;
	pub virtual std::string getActiveZoneName() = 0;
	pub virtual void setActiveZone(
		std::string zoneName, std::shared_ptr<worldGame3D::ZoneStartExtraData> zoneStartExtraData, bool restartZoneIfActive,
		bool useDelayedOnLoopActivation) = 0;

    pub ~IGamePartContentZones() override = default;
};
};
