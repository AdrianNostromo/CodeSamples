#pragma once

#include <base/gh.h>
#include "PhysicsShapeConfig.h"
#include <base/math/Vector3.h>

namespace physics3D {
class PhysicsShapeConfigBox : public PhysicsShapeConfig {priv typedef PhysicsShapeConfig super;pub dCtor(PhysicsShapeConfigBox);
	pub static int TYPE;

	pub float collisionMargin;
	// Similar to a radius logic.
	pub Vector3 halfExtents;

    pub explicit PhysicsShapeConfigBox(
		float collisionMargin,
		Vector3 halfExtents);

    pub ~PhysicsShapeConfigBox() override;
};
};
