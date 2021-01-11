#pragma once

#include <base/gh.h>
#include "IToolMDPadLocalFrontBack.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
    
namespace rpg3D {
namespace playerUnit {
class ToolMDPLFB_Base : public rpg3D::Tool, virtual public IToolMDPadLocalFrontBack {priv typedef rpg3D::Tool super;pub dCtor(ToolMDPLFB_Base);
    priv static ToolConfigBase* ToolConfig;

    pub explicit ToolMDPLFB_Base();

    pub bool canTrigger() final;

    pub ~ToolMDPLFB_Base() override;
};
};
};
