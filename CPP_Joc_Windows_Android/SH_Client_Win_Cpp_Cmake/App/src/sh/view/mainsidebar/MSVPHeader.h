#pragma once

#include <base/gh.h>
#include "MSVPH_LoggedIn.h"

namespace sh {
class MSVPHeader : public MSVPH_LoggedIn {priv typedef MSVPH_LoggedIn super; pub dCtor(MSVPHeader);
	prot static MenuItemConfig* new_viewItemConfig_panel_header();
    
	pub explicit MSVPHeader(
        IApp* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList,
        worldGame3D::IAppGame* appGame, IAppUtils* appUtils);

	pub ~MSVPHeader() override;
};
};
