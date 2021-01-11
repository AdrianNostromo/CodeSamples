#pragma once

#include <base/gh.h>
#include "EEVSDA_Body.h"
#include <graphics/visual2d/drawable/IInteractiveDrawable2D.h>
#include <graphics/visual2d/drawable/SpriteButton2D.h>

namespace rpg3D {
class EEVSettingsDrawerA : public EEVSDA_Body {priv typedef EEVSDA_Body super; pub dCtor(EEVSettingsDrawerA);
	prot static MenuItemConfig* new_viewItemConfig_settingsDrawerA();

	priv CbTouchListener touchListener{};

	priv IInteractiveDrawable2D* taScreen;
	
	pub explicit EEVSettingsDrawerA(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	priv bool v2d_onCursorDown(
		IInteractiveDrawable2D* target,
		int cursorIndex, base::Touch::ButtonCode* buttonId,
		Vector2& screenPos, Vector2& localPos);

	prot void createVisuals(Map1D<std::string, void*>* createConfigMap) override;

	pub ~EEVSettingsDrawerA() override;

};
};
