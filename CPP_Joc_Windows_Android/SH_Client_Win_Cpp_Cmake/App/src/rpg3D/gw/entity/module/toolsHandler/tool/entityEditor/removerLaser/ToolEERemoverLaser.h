#pragma once

#include <base/gh.h>
#include "TEERL_Tool.h"

namespace rpg3D {
class ToolEERemoverLaser : public TEERL_Tool {priv typedef TEERL_Tool super;pub dCtor(ToolEERemoverLaser);
    pub explicit ToolEERemoverLaser(ToolConfigEERemoverLaser* config, std::shared_ptr<ExtraData> extraData);

    pub ~ToolEERemoverLaser() override;
};
};
