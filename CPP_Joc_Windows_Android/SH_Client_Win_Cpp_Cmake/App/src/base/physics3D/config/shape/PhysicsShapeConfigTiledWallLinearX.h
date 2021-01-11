#pragma once

#include <base/gh.h>
#include "PhysicsShapeConfig.h"
#include <base/math/Vector3.h>

namespace physics3D {
class PhysicsShapeConfigTiledWallLinearX : public PhysicsShapeConfig {priv typedef PhysicsShapeConfig super;pub dCtor(PhysicsShapeConfigTiledWallLinearX);
	pub static int TYPE;

	pub float collisionMargin;
	// The pShape will be a 3d object, it requires a Vector3 size.
	pub Vector3 tileSize;
    
	pub explicit PhysicsShapeConfigTiledWallLinearX(
		float collisionMargin,
		Vector3 tileSize);

    pub ~PhysicsShapeConfigTiledWallLinearX() override;
};
};
