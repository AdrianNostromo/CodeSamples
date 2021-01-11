#pragma once

#include <base/gh.h>
#include "GPCZ_Base.h"

namespace base {
	class GWEvent;
};
namespace worldGame3D {
	class ContentZone;
};

namespace worldGame3D {
class GPCZ_ActiveZone : public GPCZ_Base {priv typedef GPCZ_Base super;pub dCtor(GPCZ_ActiveZone);
	priv std::shared_ptr<ContentZone> activeZone = nullptr;
	priv bool isActiveZoneFinishedLoading = true;

	priv std::string delayedZoneActivation_zoneName;
	priv std::shared_ptr<worldGame3D::ZoneStartExtraData> delayedZoneActivation_zoneStartExtraData;
	priv bool delayedZoneActivation_restartZoneIfActive;
	priv bool isDelayedZoneActivation = false;

    pub explicit GPCZ_ActiveZone(base::IGameWorld* gw);

	pub std::shared_ptr<ContentZone> getActiveContentZone() final;
	pub std::string getActiveZoneName() final;
	pub void setActiveZone(
		std::string zoneName, std::shared_ptr<worldGame3D::ZoneStartExtraData> zoneStartExtraData, bool restartZoneIfActive,
		bool useDelayedOnLoopActivation) final;
	prot virtual void handleMainUIViewSwitching(std::shared_ptr<ContentZone> activeZone) = 0;
	
	priv void onGWEvent_gameLoop_post(IEventListener& eventListener, base::GWEvent& event);

	prot void disposePre() override;
	pub ~GPCZ_ActiveZone() override;
};
};
