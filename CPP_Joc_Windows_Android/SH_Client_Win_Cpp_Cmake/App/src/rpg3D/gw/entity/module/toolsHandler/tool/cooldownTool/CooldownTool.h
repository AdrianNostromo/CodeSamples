#pragma once

#include <base/gh.h>
#include "CT_Actions.h"

namespace rpg3D {
class CooldownTool : public CT_Actions {priv typedef CT_Actions super;pub dCtor(CooldownTool);
	
    pub explicit CooldownTool(ToolConfigPersistentCooldown* config);

    pub ~CooldownTool() override;
};
};
