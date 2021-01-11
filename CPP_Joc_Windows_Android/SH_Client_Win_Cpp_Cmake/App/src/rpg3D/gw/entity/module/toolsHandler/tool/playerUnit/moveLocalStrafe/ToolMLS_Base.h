#pragma once

#include <base/gh.h>
#include "IToolMLocalStrafe.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
    
namespace rpg3D {
namespace playerUnit {
class ToolMLS_Base : public rpg3D::Tool, virtual public IToolMLocalStrafe {priv typedef rpg3D::Tool super;pub dCtor(ToolMLS_Base);
    priv static ToolConfigBase* ToolConfig;

    pub explicit ToolMLS_Base();

    pub bool canTrigger() final;

    pub ~ToolMLS_Base() override;
};
};
};
