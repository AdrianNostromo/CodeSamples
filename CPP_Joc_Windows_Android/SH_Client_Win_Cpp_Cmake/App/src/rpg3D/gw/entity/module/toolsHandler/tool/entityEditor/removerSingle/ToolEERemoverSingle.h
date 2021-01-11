#pragma once

#include <base/gh.h>
#include "TEERS_Tool.h"

namespace rpg3D {
class ToolEERemoverSingle : public TEERS_Tool {priv typedef TEERS_Tool super;pub dCtor(ToolEERemoverSingle);
    pub explicit ToolEERemoverSingle(ToolConfigEERemoverSingle* config, std::shared_ptr<ExtraData> extraData);

    pub ~ToolEERemoverSingle() override;
};
};
