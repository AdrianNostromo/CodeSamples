#pragma once

#include <base/gh.h>
#include "PhysicsShapeConfig.h"
#include <base/math/Vector3.h>

namespace physics3D {
class PhysicsShapeConfigCylinder : public PhysicsShapeConfig {priv typedef PhysicsShapeConfig super;pub dCtor(PhysicsShapeConfigCylinder);
	pub static int TYPE;

	pub float collisionMargin;
	
	pub Vector3 mainAxis;

	pub Vector3 halfExtents;

    pub explicit PhysicsShapeConfigCylinder(
		float collisionMargin,
		Vector3& mainAxis,
		Vector3 halfExtents);

    pub ~PhysicsShapeConfigCylinder() override;
};
};
