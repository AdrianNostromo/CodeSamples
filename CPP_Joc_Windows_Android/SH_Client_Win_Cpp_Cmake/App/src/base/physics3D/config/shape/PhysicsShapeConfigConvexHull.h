#pragma once

#include <base/gh.h>
#include "PhysicsShapeConfig.h"
#include <base/list/Array1D.h>
#include <base/math/Vector3.h>

namespace physics3D {
class PhysicsShapeConfigConvexHull : public PhysicsShapeConfig {priv typedef PhysicsShapeConfig super;pub dCtor(PhysicsShapeConfigConvexHull);
	pub class Vec3 {
		pub float x;
		pub float y;
		pub float z;
	};
	
	pub static int TYPE;

	pub float collisionMargin;
	pub Array1D<Vec3> points;
    
	pub explicit PhysicsShapeConfigConvexHull(
		float collisionMargin,
		int pointsCount, Vec3* pointsData);

    pub ~PhysicsShapeConfigConvexHull() override;
};
};
