#pragma once

#include <base/gh.h>
#include "MovementConfig.h"

namespace rpg3D {
class MovementConfigDirect : public MovementConfig {priv typedef MovementConfig super;pub dCtor(MovementConfigDirect);
    pub static const int TYPE;

    pub float moveSpeedS;

    pub explicit MovementConfigDirect(
        float moveSpeedS);

    pub ~MovementConfigDirect() override;
};
};
