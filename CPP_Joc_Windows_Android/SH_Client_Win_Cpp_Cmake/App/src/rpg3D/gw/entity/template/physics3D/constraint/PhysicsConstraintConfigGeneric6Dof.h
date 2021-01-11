#pragma once

#include <base/gh.h>
#include "PhysicsConstraintConfig.h"
#include <base/math/Vector3.h>

namespace rpg3D {
class PhysicsConstraintConfigGeneric6Dof : public PhysicsConstraintConfig {priv typedef PhysicsConstraintConfig super;pub dCtor(PhysicsConstraintConfigGeneric6Dof);
	pub class Limit {/*pub dCtor(Limit);*/
		// For angular limits, using values larger than +-PI, use the special btScalar pi value.
		pub enum Type {Custom, FreeSpinning};
		pub Type type;

		pub float customMin;
		pub float customMax;

		pub explicit Limit(Type type)
			: type(type), customMin(0.0f), customMax(0.0f)
		{}
		pub explicit Limit(float customMin, float customMax)
			: type(Type::Custom), customMin(customMin), customMax(customMax)
		{}
	};
	pub static int TYPE;

	pub int pBodyIndexA;
	pub int pBodyIndexB;

	pub Vector3 framePosInA;
	pub Vector3 framePosInB;

	pub Limit limitPosX;
	pub Limit limitPosY;
	pub Limit limitPosZ;
	
	pub Limit limitAngX;
	pub Limit limitAngY;
	pub Limit limitAngZ;

    pub explicit PhysicsConstraintConfigGeneric6Dof(
		int pBodyIndexA, int pBodyIndexB,
		Vector3 framePosInA, Vector3 framePosInB,
		Limit limitPosX, Limit limitPosY, Limit limitPosZ,
		Limit limitAngX, Limit limitAngY, Limit limitAngZ,
		bool disableCollisionsBetweenLinkedBodies);

    pub ~PhysicsConstraintConfigGeneric6Dof() override;
};
};
