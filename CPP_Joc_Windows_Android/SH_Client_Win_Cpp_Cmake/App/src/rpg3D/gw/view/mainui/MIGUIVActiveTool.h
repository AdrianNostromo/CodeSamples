#pragma once

#include <base/gh.h>
#include "MIGUIVAT_Indicator.h"

namespace rpg3D {
class MIGUIVActiveTool : public MIGUIVAT_Indicator {priv typedef MIGUIVAT_Indicator super;pub dCtor(MIGUIVActiveTool);
	prot static MenuItemConfig* new_viewItemConfig_activeTool();

    pub explicit MIGUIVActiveTool(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);

    pub ~MIGUIVActiveTool() override;
};
};
