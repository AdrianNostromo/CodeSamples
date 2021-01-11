#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/ToolEntityEditor.h>

namespace rpg3D {
	class ToolConfigEERemoverLaser;
};

namespace rpg3D {
class TEERL_Base : public ToolEntityEditor {priv typedef ToolEntityEditor super;pub dCtor(TEERL_Base);
    prot ToolConfigEERemoverLaser* config;

    pub explicit TEERL_Base(ToolConfigEERemoverLaser* config, std::shared_ptr<ExtraData> extraData);

    pub ~TEERL_Base() override;
};
};
