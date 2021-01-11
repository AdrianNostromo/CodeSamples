#pragma once

#include <base/gh.h>
#include "../ToolZoneEditor.h"

namespace rpg3D {
	class ToolConfigZEGroundTiled;
};

namespace rpg3D {
class TZEGT_Base : public ToolZoneEditor {priv typedef ToolZoneEditor super;pub dCtor(TZEGT_Base);
    prot ToolConfigZEGroundTiled* config;

    pub explicit TZEGT_Base(ToolConfigZEGroundTiled* config, std::shared_ptr<ExtraData> extraData);

    pub ~TZEGT_Base() override;
};
};
