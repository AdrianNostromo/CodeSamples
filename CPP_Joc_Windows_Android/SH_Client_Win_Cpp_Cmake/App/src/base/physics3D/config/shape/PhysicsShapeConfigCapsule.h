#pragma once

#include <base/gh.h>
#include "PhysicsShapeConfig.h"
#include <base/math/Vector3.h>

namespace physics3D {
class PhysicsShapeConfigCapsule : public PhysicsShapeConfig {priv typedef PhysicsShapeConfig super;pub dCtor(PhysicsShapeConfigCapsule);
	pub static int TYPE;

	// Capsules don't use margin. Their entire radius == margin;
	//pub float collisionMargin;
	
	pub Vector3 mainAxis;

	// Note. total_height = endDomesRadius + cylinderPartHeight + endDomesRadius
	pub float endDomesRadius;
	pub float cylinderPartHeight;

    pub explicit PhysicsShapeConfigCapsule(
		Vector3& mainAxis,
		float endDomesRadius, float cylinderPartHeight);

    pub ~PhysicsShapeConfigCapsule() override;
};
};
