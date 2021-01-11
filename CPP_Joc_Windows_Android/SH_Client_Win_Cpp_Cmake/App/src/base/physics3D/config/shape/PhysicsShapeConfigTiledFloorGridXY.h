#pragma once

#include <base/gh.h>
#include "PhysicsShapeConfig.h"
#include <base/math/Vector2.h>

namespace physics3D {
class PhysicsShapeConfigTiledFloorGridXY : public PhysicsShapeConfig {priv typedef PhysicsShapeConfig super;pub dCtor(PhysicsShapeConfigTiledFloorGridXY);
	pub static int TYPE;

	pub float collisionMargin;
	// The pShape will be a 2d_xy plane object, it requires a Vector2 size.
	pub Vector2 tileSize;
    
	pub explicit PhysicsShapeConfigTiledFloorGridXY(
		float collisionMargin,
		Vector2 tileSize);

    pub ~PhysicsShapeConfigTiledFloorGridXY() override;
};
};
