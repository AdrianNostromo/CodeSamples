#pragma once

#include <base/interaction/CbTouchListener.h>
#include "MIGUIV_DirectionalPad.h"

class IInteractiveDrawable2D;

namespace rpg3D {
class MainInGameUIView : public MIGUIV_DirectionalPad {priv typedef MIGUIV_DirectionalPad super;pub dCtor(MainInGameUIView);
    prot static ArrayList<MenuItemConfig*>* viewItemConfigs;

    prot static ArrayList<StateChangeDurations*>* viewAnimationDurations;

    prot bool isInitialSync = false;

    prot IInteractiveDrawable2D* pixIcon;
    prot IInteractiveDrawable2D* bgControlModeToggle;

    priv CbTouchListener touchListener{};

	pub explicit MainInGameUIView(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);
    pub explicit MainInGameUIView(IApp* app, base::IGameWorld* gw);
    prot void createContent() override;

    priv bool onCursorDown(
        IInteractiveDrawable2D* target,
        int cursorIndex, base::Touch::ButtonCode* buttonId,
        Vector2& screenPos, Vector2& localPos);

    pub ~MainInGameUIView() override;
};
};
