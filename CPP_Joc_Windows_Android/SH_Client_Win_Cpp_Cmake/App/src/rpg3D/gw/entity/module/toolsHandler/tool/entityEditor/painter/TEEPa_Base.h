#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/ToolEntityEditor.h>

namespace rpg3D {
	class ToolConfigEEPainter;
};

namespace rpg3D {
class TEEPa_Base : public ToolEntityEditor {priv typedef ToolEntityEditor super;pub dCtor(TEEPa_Base);
    prot ToolConfigEEPainter* config;

    pub explicit TEEPa_Base(ToolConfigEEPainter* config, std::shared_ptr<ExtraData> extraData);

    pub ~TEEPa_Base() override;
};
};
