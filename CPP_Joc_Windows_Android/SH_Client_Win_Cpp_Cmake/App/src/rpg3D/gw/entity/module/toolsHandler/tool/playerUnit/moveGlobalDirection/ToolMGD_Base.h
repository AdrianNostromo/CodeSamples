#pragma once

#include <base/gh.h>
#include "IToolMGlobalDirection.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
    
namespace rpg3D {
namespace playerUnit {
class ToolMGD_Base : public rpg3D::Tool, virtual public IToolMGlobalDirection {priv typedef rpg3D::Tool super;pub dCtor(ToolMGD_Base);
    priv static ToolConfigBase* ToolConfig;

    pub explicit ToolMGD_Base();

    pub bool canTrigger() final;

    pub ~ToolMGD_Base() override;
};
};
};
