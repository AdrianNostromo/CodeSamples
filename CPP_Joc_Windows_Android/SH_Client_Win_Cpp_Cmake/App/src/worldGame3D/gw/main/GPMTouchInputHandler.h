#pragma once

#include <base/interaction/BufferedTouchListener.h>
#include <graphics/visual2d/drawable/TouchArea2D.h>
#include <worldGame3D/gw/main/util/IScreenTouch2DListener.h>
#include "GPMTouchable3DEntities.h"
#include <base/screen/IScreen.h>

namespace base {
class GPMTouchInputHandler : public GPMTouchable3DEntities {priv typedef GPMTouchable3DEntities super;pub dCtor(GPMTouchInputHandler);
    // This must be initialised with 0 and modified using the setters.
    priv IScreenTouch2DListener* screenTouch2DListener = nullptr;

	priv BufferedTouchListener touchListenerMove{false, false, false, false, true, false, false};
	priv BufferedTouchListener touchListenerDrag{false, false, false, true, false, false, false};
	priv BufferedTouchListener touchListenerButtons{true, true, true, false, false, false, false};

    // There are 2 touch areas so that the move event is always received even on drags.
	// The reason for a touch_area just for move is that is it over the dPad so move events will dispatch even if the dPad is draged. THe dPad will block button presses under it so it cannot be made with touch_fallthrouch enabled.
	priv TouchArea2D* touchAreaBgMove = nullptr;
	priv TouchArea2D* touchAreaBgButtons = nullptr;

	priv base::IScreen::CbListener screenListener{"GPMTouchInputHandler"};

	pub explicit GPMTouchInputHandler(base::IGameWorld* gw);
	prot void createMain() override;
    pub void createContent() override;

	pub void setScreenTouch2DListener(IScreenTouch2DListener* screenTouch2DListener) override;

	priv void onGWEvent_gameLoop_localInput(IEventListener& eventListener, base::GWEvent& event);
    pub void isPlayEnabled_changedEvent() override;

	priv void onActiveScreenResolutionChange(int resolutionX, int resolutionY);

	priv bool onCursorEvent(TouchEventType* touchEventType, int cursorIndex, base::Touch::ButtonCode* buttonId, float screenX, float screenY);

	pub ~GPMTouchInputHandler() override;
};
};
