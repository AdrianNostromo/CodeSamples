#pragma once

#include <base/gh.h>
#include "PhysicsShapeConfig.h"
#include <base/math/Vector3.h>

namespace physics3D {
class PhysicsShapeConfigSphere : public PhysicsShapeConfig {priv typedef PhysicsShapeConfig super;pub dCtor(PhysicsShapeConfigSphere);
	pub static int TYPE;

	// For spheres, the radius is fake and the radius is used as a collission margin.
	// DON'T OVERRIDE the sphere collission margin (check the manual).
	//pub float collisionMargin;

	// Manually add collission margin to this.
	pub float radius;

    pub explicit PhysicsShapeConfigSphere(float radius);

    pub ~PhysicsShapeConfigSphere() override;
};
};
