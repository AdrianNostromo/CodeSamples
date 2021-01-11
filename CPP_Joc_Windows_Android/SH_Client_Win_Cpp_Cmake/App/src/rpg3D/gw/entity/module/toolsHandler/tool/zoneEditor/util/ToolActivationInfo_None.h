#pragma once

#include <base/gh.h>
#include "ToolActivationInfo_Base.h"
#include <base/math/Vector2Int.h>

namespace rpg3D {
class ToolActivationInfo_None : public ToolActivationInfo_Base {priv typedef ToolActivationInfo_Base super;pub dCtor(ToolActivationInfo_None);
    pub static const int TYPE;

    pub explicit ToolActivationInfo_None(int cursorIndex);

    pub ~ToolActivationInfo_None() override;
};
};
