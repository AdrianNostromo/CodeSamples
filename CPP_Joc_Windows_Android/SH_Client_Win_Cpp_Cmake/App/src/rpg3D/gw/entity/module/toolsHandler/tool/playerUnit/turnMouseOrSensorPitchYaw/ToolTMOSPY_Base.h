#pragma once

#include <base/gh.h>
#include "IToolTMouseOrSensorPitchYaw.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
    
namespace rpg3D {
namespace playerUnit {
class ToolTMOSPY_Base : public rpg3D::Tool, virtual public IToolTMouseOrSensorPitchYaw {priv typedef rpg3D::Tool super;pub dCtor(ToolTMOSPY_Base);
    priv static ToolConfigBase* ToolConfig;

    pub explicit ToolTMOSPY_Base();

    pub bool canTrigger() final;

    pub ~ToolTMOSPY_Base() override;
};
};
};
