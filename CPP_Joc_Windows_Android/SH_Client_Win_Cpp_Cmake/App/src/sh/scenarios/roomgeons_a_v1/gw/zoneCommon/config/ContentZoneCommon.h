#pragma once

#include <base/gh.h>
#include <rpg3D/gw/contentZones/zone/ContentZone.h>

namespace roomgeons_a_v1 {
namespace common {
class ContentZoneCommon : public rpg3D::ContentZone {priv typedef rpg3D::ContentZone super;pub dCtor(ContentZoneCommon);
	pub explicit ContentZoneCommon(std::string& id);

	pub int getMainUIViewID() final;
	pub rpg3D::MainInGameUIView* getNewMainUIViewUninitialised(base::IGameWorld* gw) final;

	prot void onZoneActivated(std::shared_ptr<worldGame3D::ZoneStartExtraData>& zoneStartExtraData, base::IGameWorld* gw) override;
	prot void onZoneDeactivate_pre(base::IGameWorld* gw) override;

    pub ~ContentZoneCommon() override;
};
};
};
