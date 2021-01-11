#pragma once

#include <base/gh.h>
#include "CZ_Base.h"

namespace rpg3D {
class ContentZone : public CZ_Base {priv typedef CZ_Base super;pub dCtor(ContentZone);
	priv bool useAutoPlayerPositioning;
	priv std::shared_ptr<std::string> defaultZoneEntrance;

    pub explicit ContentZone(
		std::string& id,
		bool useAutoPlayerPositioning=false, std::shared_ptr<std::string> defaultZoneEntrance=nullptr);

	prot void onZoneActivatedPositionPlayer(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;

	pub ~ContentZone() override;
};
};
