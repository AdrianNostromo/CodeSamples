#pragma once

#include <base/gh.h>
#include "IToolMTGridTurnBased.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
    
namespace rpg3D {
namespace playerUnit {
class ToolMTGTB_Base : public rpg3D::Tool, virtual public IToolMTGridTurnBased {priv typedef rpg3D::Tool super;pub dCtor(ToolMTGTB_Base);
    priv static ToolConfigBase* ToolConfig;

    pub explicit ToolMTGTB_Base();

    pub bool canTrigger() final;

    pub ~ToolMTGTB_Base() override;
};
};
};
