#pragma once

#include <base/gh.h>
#include "THM_SelectedTools.h"

namespace rpg3D {
class THM_Activation : public THM_SelectedTools {
private: typedef THM_SelectedTools super;
public:
   pub dCtor(THM_Activation);
    pub explicit THM_Activation(
		IWorldEntity* entity,
		ToolsHandlerTemplate* t);

    pub ~THM_Activation() override;
};
};
