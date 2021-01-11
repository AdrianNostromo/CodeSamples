#pragma once

#include <base/gh.h>
#include "MDIN_ScreenTouch2D.h"

namespace rpg3D {
class MDIN_Move_Base : public MDIN_ScreenTouch2D {priv typedef MDIN_ScreenTouch2D super; pub dCtor(MDIN_Move_Base);
	prot Vector2 keyboardDirVector{};

	prot Vector2 uiDPadDirVector{};

	priv Vector2 lastDispatchedMoveDirVector2D{};

	pub explicit MDIN_Move_Base(base::IGameWorld* gw);

	prot void onMoveInput();

    pub ~MDIN_Move_Base() override;
};
};
