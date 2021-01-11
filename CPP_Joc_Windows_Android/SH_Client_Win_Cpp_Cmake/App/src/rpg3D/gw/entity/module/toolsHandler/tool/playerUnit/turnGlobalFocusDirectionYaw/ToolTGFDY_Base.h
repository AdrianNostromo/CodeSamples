#pragma once

#include <base/gh.h>
#include "IToolTGlobalFocusDirectionYaw.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
    
namespace rpg3D {
namespace playerUnit {
class ToolTGFDY_Base : public rpg3D::Tool, virtual public IToolTGlobalFocusDirectionYaw {priv typedef rpg3D::Tool super;pub dCtor(ToolTGFDY_Base);
    priv static ToolConfigBase* ToolConfig;
    
    pub explicit ToolTGFDY_Base();

    pub bool canTrigger() final;

    pub ~ToolTGFDY_Base() override;
};
};
};
