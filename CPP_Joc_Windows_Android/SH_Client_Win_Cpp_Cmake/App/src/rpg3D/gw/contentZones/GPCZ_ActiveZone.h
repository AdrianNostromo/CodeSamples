#pragma once

#include <base/gh.h>
#include "GPCZ_Base.h"

namespace rpg3D {
class GPCZ_ActiveZone : public GPCZ_Base {priv typedef GPCZ_Base super;pub dCtor(GPCZ_ActiveZone);
	priv int activeMainUIVIewID = -1;

    pub explicit GPCZ_ActiveZone(base::IGameWorld* gw);
	
	prot void handleMainUIViewSwitching(std::shared_ptr<worldGame3D::ContentZone> activeZone) final;

	pub ~GPCZ_ActiveZone() override;
};
};
