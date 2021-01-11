#pragma once

#include <base/gh.h>
#include "ToolActivationInfo_Base.h"
#include <base/math/Vector3.h>

namespace rpg3D {
class ToolActivationInfo_GroundPos : public ToolActivationInfo_Base {priv typedef ToolActivationInfo_Base super;pub dCtor(ToolActivationInfo_GroundPos);
    pub static const int TYPE;

    pub Vector3 groundPos;

    pub explicit ToolActivationInfo_GroundPos(int cursorIndex, Vector3& groundPos);

    pub ~ToolActivationInfo_GroundPos() override;
};
};
