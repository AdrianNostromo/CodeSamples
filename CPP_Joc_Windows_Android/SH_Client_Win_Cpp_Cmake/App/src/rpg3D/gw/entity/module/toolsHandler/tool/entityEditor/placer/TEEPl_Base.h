#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/ToolEntityEditor.h>

namespace rpg3D {
	class ToolConfigEEPlacer;
};

namespace rpg3D {
class TEEPl_Base : public ToolEntityEditor {priv typedef ToolEntityEditor super;pub dCtor(TEEPl_Base);
    prot ToolConfigEEPlacer* config;

    pub explicit TEEPl_Base(ToolConfigEEPlacer* config, std::shared_ptr<ExtraData> extraData);

    pub ~TEEPl_Base() override;
};
};
