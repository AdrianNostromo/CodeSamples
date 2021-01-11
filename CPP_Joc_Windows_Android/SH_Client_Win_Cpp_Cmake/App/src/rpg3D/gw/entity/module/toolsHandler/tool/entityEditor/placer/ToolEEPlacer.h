#pragma once

#include <base/gh.h>
#include "TEEPl_Tool.h"

namespace rpg3D {
class ToolEEPlacer : public TEEPl_Tool {priv typedef TEEPl_Tool super;pub dCtor(ToolEEPlacer);
    pub explicit ToolEEPlacer(ToolConfigEEPlacer* config, std::shared_ptr<ExtraData> extraData);

    pub ~ToolEEPlacer() override;
};
};
