#pragma once

#include <base/gh.h>
#include "IToolMGlobalDestination_TMoveDirection.h"
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>
    
namespace rpg3D {
namespace playerUnit {
class ToolMGDTMD_Base : public rpg3D::Tool, virtual public IToolMGlobalDestination_TMoveDirection {priv typedef rpg3D::Tool super;pub dCtor(ToolMGDTMD_Base);
    priv static ToolConfigBase* ToolConfig;

    pub enum DestinationType { GroundAndSomeEntities, GroundOnly };

    prot int movementTypeOverride;
    prot bool usesGridMovement;
    prot DestinationType destinationType;

    pub explicit ToolMGDTMD_Base(int movementTypeOverride, bool usesGridMovement, DestinationType destinationType);

    pub bool canTrigger() final;

    pub ~ToolMGDTMD_Base() override;
};
};
};
