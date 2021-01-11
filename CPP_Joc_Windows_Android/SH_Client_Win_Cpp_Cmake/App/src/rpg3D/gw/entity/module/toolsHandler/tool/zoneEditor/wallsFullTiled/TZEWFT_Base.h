#pragma once

#include <base/gh.h>
#include "../ToolZoneEditor.h"

namespace rpg3D {
	class ToolConfigZEWallsFullTiled;
};

namespace rpg3D {
class TZEWFT_Base : public ToolZoneEditor {priv typedef ToolZoneEditor super;pub dCtor(TZEWFT_Base);
    prot ToolConfigZEWallsFullTiled* config;

    pub explicit TZEWFT_Base(ToolConfigZEWallsFullTiled* config, std::shared_ptr<ExtraData> extraData);

    pub ~TZEWFT_Base() override;
};
};
