#pragma once

#include <base/gh.h>
#include "MSVP_AccountSettingsDimmer.h"
#include <base/interaction/CbTouchListener.h>

namespace sh {
class MSVPanel : public MSVP_AccountSettingsDimmer {priv typedef MSVP_AccountSettingsDimmer super; pub dCtor(MSVPanel);
	prot static MenuItemConfig* new_viewItemConfig_panel();

	priv CbTouchListener touchListener{};

	priv IInteractiveDrawable2D* taBg = nullptr;

	pub explicit MSVPanel(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);
	prot void createContent() override;

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	pub ~MSVPanel() override;
};
};
