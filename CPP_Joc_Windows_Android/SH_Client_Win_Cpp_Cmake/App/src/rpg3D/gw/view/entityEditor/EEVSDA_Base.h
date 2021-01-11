#pragma once

#include <base/gh.h>
#include "EEV_ToolbarColors.h"

namespace rpg3D {
class EEVSDA_Base : public EEV_ToolbarColors {priv typedef EEV_ToolbarColors super; pub dCtor(EEVSDA_Base);
	priv bool isSettingsDrawerAEnabled = false;

    pub explicit EEVSDA_Base(
		IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
		IAppUtils* appUtils);

	prot void hideSettingsDrawerA();
	prot void toggleSettingsDrawerA();

	pub std::string computeStateFlags(std::string baseState) override;

    pub ~EEVSDA_Base() override;
};
};
