#pragma once

#include "DeV_PageGMan.h"
#include <base/list/Array1D.h>
#include <graphics/visual2d/drawable/TouchArea2D.h>
#include <base/interaction/CbTouchListener.h>

class ISprite2D;

namespace base {
class DebugView : public DeV_PageGMan {pub dCtor(DebugView);priv typedef DeV_PageGMan super;
	priv static ArrayList<MenuItemConfig*>* viewItemConfigs;

	priv static ArrayList<StateChangeDurations*>* viewAnimationDurations;

	priv TouchArea2D* debugTa = nullptr;
	priv ISprite2D* debugBg = nullptr;
	priv ISprite2D* debugIcon = nullptr;
	priv TouchArea2D* secretDebugTa = nullptr;

	priv bool isSecretMode = false;
	priv int secretBtnPressesCounter = 0;
	priv float lastSecretBtnPressDeltaS = 0.0f;

	// Once this reaches 0.0f, it fades out until -1.0f is reached.
	priv float btnFadeOutStartDelayS = 0.0f;

	priv CbTouchListener touchListener{};

    pub explicit DebugView(IApp* app);
	prot void createContent() override;

	priv bool onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	prot void visualLoop(float deltaS, int deltaMS) override;

	pub std::string computeStateFlags(std::string baseState) override;

    pub ~DebugView() override;
};
};
