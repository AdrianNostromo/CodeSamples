#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/main/util/CbScreenTouch2DListener.h>
#include "MDIN_Base.h"

namespace rpg3D {
class MDIN_ScreenTouch2D : public MDIN_Base {priv typedef MDIN_Base super; pub dCtor(MDIN_ScreenTouch2D);
	priv base::CbScreenTouch2DListener screenTouch2DListener{};

    pub explicit MDIN_ScreenTouch2D(base::IGameWorld* gw);
	prot void createMain() override;
	prot void createBMain() override;

	pub void clearInput() override;

	priv void onScreenTouch2D(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, float screenX, float screenY);

	prot void disposePre() override;
	pub ~MDIN_ScreenTouch2D() override;
};
};
