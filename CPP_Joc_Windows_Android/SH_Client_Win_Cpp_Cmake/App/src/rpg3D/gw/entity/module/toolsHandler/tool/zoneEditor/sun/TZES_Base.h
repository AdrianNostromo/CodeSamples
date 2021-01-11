#pragma once

#include <base/gh.h>
#include "../ToolZoneEditor.h"

namespace rpg3D {
	class ToolConfigZESun;
};

namespace rpg3D {
class TZES_Base : public ToolZoneEditor {priv typedef ToolZoneEditor super;pub dCtor(TZES_Base);
    prot ToolConfigZESun* config;

    pub explicit TZES_Base(ToolConfigZESun* config, std::shared_ptr<ExtraData> extraData);

    pub ~TZES_Base() override;
};
};
