#pragma once

#include <base/gh.h>
#include "IToolTDPASSY_LMOSP.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
    
namespace rpg3D {
namespace playerUnit {
class ToolTDPASSYLMOSP_Base : public rpg3D::Tool, virtual public IToolTDPASSY_LMOSP {priv typedef rpg3D::Tool super;pub dCtor(ToolTDPASSYLMOSP_Base);
    priv static ToolConfigBase* ToolConfig;

    pub explicit ToolTDPASSYLMOSP_Base();

    pub bool canTrigger() final;

    pub ~ToolTDPASSYLMOSP_Base() override;
};
};
};
