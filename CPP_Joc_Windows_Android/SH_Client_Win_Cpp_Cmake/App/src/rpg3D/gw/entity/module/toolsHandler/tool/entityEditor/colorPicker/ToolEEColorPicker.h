#pragma once

#include <base/gh.h>
#include "TEECPi_Tool.h"

namespace rpg3D {
class ToolEEColorPicker : public TEECPi_Tool {priv typedef TEECPi_Tool super;pub dCtor(ToolEEColorPicker);
    pub explicit ToolEEColorPicker(ToolConfigEEColorPicker* config, std::shared_ptr<ExtraData> extraData);

    pub ~ToolEEColorPicker() override;
};
};
