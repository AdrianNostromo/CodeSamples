#pragma once

#include <base/gh.h>
#include "IToolMGlobalDirection_TMoveDirection.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
    
namespace rpg3D {
namespace playerUnit {
class ToolMGDiTMD_Base : public rpg3D::Tool, virtual public IToolMGlobalDirection_TMoveDirection {priv typedef rpg3D::Tool super;pub dCtor(ToolMGDiTMD_Base);
    priv static ToolConfigBase* ToolConfig;

    pub explicit ToolMGDiTMD_Base();

    pub bool canTrigger() final;

    pub ~ToolMGDiTMD_Base() override;
};
};
};
