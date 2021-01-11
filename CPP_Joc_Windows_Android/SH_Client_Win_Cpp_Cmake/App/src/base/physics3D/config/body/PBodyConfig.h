#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>

class Quaternion;

namespace physics3D {
	class PhysicsShapeConfig;
};
namespace rpg3D {
	class PhysicsCollissionConfig;
};

namespace rpg3D {
class PBodyConfig {pub dCtor(PBodyConfig);
	pub class UssageBitGroups {
		pub static const int None;
		pub static const int Movement;
		pub static const int CardinalRotation;
		pub static const int VisualsUpdate;
	};
	pub enum DynamicsType { Static, Kinematic, Dynamic };

	pub int ussageBitMask;

	pub DynamicsType dynamicsType;

	pub Vector3* physicsBodyOffsetPos;
	pub Quaternion* physicsBodyOffsetRot;

	pub physics3D::PhysicsShapeConfig* pShapeConfig;

	pub float mass;
	pub float friction;
	pub float rollingFriction;

	// {0,1}
	pub Vector3 enabledRotationAxes;

	pub float linearDamping;
	pub float angularDamping;

	pub Vector3* customGravity;

	pub explicit PBodyConfig(
		int ussageBitMask, 
		DynamicsType dynamicsType,
		Vector3* physicsBodyOffsetPos, Quaternion* physicsBodyOffsetRot,
		physics3D::PhysicsShapeConfig* pShapeConfig,
		float mass, float friction, float rollingFriction,
		Vector3 enabledRotationAxes,
		float linearDamping, float angularDamping,
		Vector3* customGravity);
};
};
