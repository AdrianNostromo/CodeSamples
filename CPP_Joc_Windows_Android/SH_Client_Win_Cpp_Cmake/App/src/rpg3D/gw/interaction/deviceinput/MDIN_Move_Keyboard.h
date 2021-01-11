#pragma once

#include <base/gh.h>
#include "MDIN_Move_Base.h"

namespace rpg3D {
class MDIN_Move_Keyboard : public MDIN_Move_Base {priv typedef MDIN_Move_Base super; pub dCtor(MDIN_Move_Keyboard);
	priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

	priv int keyCode_left = base::Keyboard::KeyCodes::KEY_A;
	priv int keyCode_right = base::Keyboard::KeyCodes::KEY_D;

	priv int keyCode_back = base::Keyboard::KeyCodes::KEY_S;
	priv int keyCode_front = base::Keyboard::KeyCodes::KEY_W;

	priv int usedKeyCodesCount = 4;
	priv int usedKeyCodes[4] {
		keyCode_left,
		keyCode_right,

		keyCode_back,
		keyCode_front
	};

	priv bool isKKeyPressed_moveLeft = false;
	priv bool isKKeyPressed_moveRight = false;
	// -1, 1
	priv int lastPressDirection_leftRight = 0;

	priv bool isKKeyPressed_moveBack = false;
	priv bool isKKeyPressed_moveFront = false;
	// -1, 1
	priv int lastPressDirection_backFront = 0;

    pub explicit MDIN_Move_Keyboard(base::IGameWorld* gw);
	prot void createMain() override;
	prot void createBMain() override;

	pub void clearInput() override;

	priv void onSensorEvent(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& sensorEvent);

	prot void disposePre() override;
	pub ~MDIN_Move_Keyboard() override;
};
};
