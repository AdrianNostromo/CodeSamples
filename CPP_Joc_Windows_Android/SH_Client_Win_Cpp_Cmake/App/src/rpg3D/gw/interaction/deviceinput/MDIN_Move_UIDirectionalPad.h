#pragma once

#include <base/gh.h>
#include <rpg3D/gw/view/mainui/IMainInGameUIViewListeners.h>
#include "MDIN_Move_Keyboard.h"

namespace rpg3D {
class MDIN_Move_UIDirectionalPad : public MDIN_Move_Keyboard {priv typedef MDIN_Move_Keyboard super; pub dCtor(MDIN_Move_UIDirectionalPad);
	priv IMainInGameUIViewListeners::CbListenerDPad uiDpadControlsListener{};

    pub explicit MDIN_Move_UIDirectionalPad(base::IGameWorld* gw);
	prot void createBMain() override;

	priv void uiDPad_onSetMoveDirection(float moveDirVectorX, float moveDirVectorY);

	prot void disposePre() override;
	pub ~MDIN_Move_UIDirectionalPad() override;
};
};
