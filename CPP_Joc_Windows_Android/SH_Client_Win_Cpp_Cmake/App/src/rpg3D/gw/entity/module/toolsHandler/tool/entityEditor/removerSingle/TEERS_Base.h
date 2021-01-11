#pragma once

#include <base/gh.h>
#include <rpg3D/gw/entity/module/toolsHandler/tool/entityEditor/ToolEntityEditor.h>

namespace rpg3D {
	class ToolConfigEERemoverSingle;
};

namespace rpg3D {
class TEERS_Base : public ToolEntityEditor {priv typedef ToolEntityEditor super;pub dCtor(TEERS_Base);
	prot ToolConfigEERemoverSingle* config;

    pub explicit TEERS_Base(ToolConfigEERemoverSingle* config, std::shared_ptr<ExtraData> extraData);

    pub ~TEERS_Base() override;
};
};
