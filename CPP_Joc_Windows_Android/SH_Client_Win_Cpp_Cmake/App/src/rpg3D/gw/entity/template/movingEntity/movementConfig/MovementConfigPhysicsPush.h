#pragma once

#include <base/gh.h>
#include "MovementConfig.h"

namespace rpg3D {
class MovementConfigPhysicsPush : public MovementConfig {priv typedef MovementConfig super;pub dCtor(MovementConfigPhysicsPush);
    pub static const int TYPE;

    pub float moveForce;
    pub float moveDamping;

    pub float stopDamping;

    pub bool applyForceToMovementPBodyOnly;

    pub explicit MovementConfigPhysicsPush(
        float moveForce, float moveDamping,
        float stopDamping,
        bool applyForceToMovementPBodyOnly);

    pub ~MovementConfigPhysicsPush() override;
};
};
