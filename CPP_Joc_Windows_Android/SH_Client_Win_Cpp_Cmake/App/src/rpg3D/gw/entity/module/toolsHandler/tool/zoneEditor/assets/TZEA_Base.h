#pragma once

#include <base/gh.h>
#include "../ToolZoneEditor.h"

namespace rpg3D {
	class ToolConfigZEAssets;
};

namespace rpg3D {
class TZEA_Base : public ToolZoneEditor {priv typedef ToolZoneEditor super;pub dCtor(TZEA_Base);
    prot ToolConfigZEAssets* config;

    pub explicit TZEA_Base(ToolConfigZEAssets* config, std::shared_ptr<ExtraData> extraData);

    pub ~TZEA_Base() override;
};
};
