#pragma once

#include <base/gh.h>
#include "MovementConfig.h"

namespace rpg3D {
class MovementConfigPhysicsRollingSphere : public MovementConfig {priv typedef MovementConfig super;pub dCtor(MovementConfigPhysicsRollingSphere);
    pub static const int TYPE;

    pub float moveAngularVelocity;
    pub float moveAngularDamping;

    pub float stopAngularDamping;

    pub explicit MovementConfigPhysicsRollingSphere(
        float moveAngularVelocity, float moveAngularDamping,
        float stopAngularDamping);

    pub ~MovementConfigPhysicsRollingSphere() override;
};
};
