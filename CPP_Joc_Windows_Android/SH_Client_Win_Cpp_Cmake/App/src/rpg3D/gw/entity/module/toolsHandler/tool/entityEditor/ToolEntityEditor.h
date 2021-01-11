#pragma once

#include <base/gh.h>
#include "TEE_Rotate.h"

namespace rpg3D {
class ToolEntityEditor : public TEE_Rotate {priv typedef TEE_Rotate super;pub dCtor(ToolEntityEditor);
    pub explicit ToolEntityEditor(ToolConfigEntityEditor* config, std::shared_ptr<ExtraData> extraData,
		bool usesAutoToolModeActivationSound);

    pub ~ToolEntityEditor() override;
};
};
