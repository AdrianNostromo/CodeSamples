#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/Tool.h>

namespace rpg3D {
    class ToolConfigPersistentCooldown;
};

namespace rpg3D {
class CT_Base : public Tool {priv typedef Tool super;pub dCtor(CT_Base);
    pub explicit CT_Base(ToolConfigPersistentCooldown* config);

    prot ToolConfigPersistentCooldown* getLocalConfig();

    pub ~CT_Base() override;
};
};
