#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/ToolEntityEditor.h>

namespace rpg3D {
	class ToolConfigEEColorPicker;
};

namespace rpg3D {
class TEECPi_Base : public ToolEntityEditor {priv typedef ToolEntityEditor super;pub dCtor(TEECPi_Base);
    prot ToolConfigEEColorPicker* config;

    pub explicit TEECPi_Base(ToolConfigEEColorPicker* config, std::shared_ptr<ExtraData> extraData);

    pub ~TEECPi_Base() override;
};
};
