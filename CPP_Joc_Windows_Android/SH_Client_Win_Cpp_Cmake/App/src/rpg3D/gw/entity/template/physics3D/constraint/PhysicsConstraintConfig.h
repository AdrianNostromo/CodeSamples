#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>

namespace rpg3D {
class PhysicsConstraintConfig {pub dCtor(PhysicsConstraintConfig);
	prot static int NewType();
	
	pub const int type;

	pub bool disableCollisionsBetweenLinkedBodies;

	// This is used to avoid dynamic cast.
	// It contains the subclass void*;
	prot void* rawSubclassPointer = nullptr;

    pub explicit PhysicsConstraintConfig(int type, bool disableCollisionsBetweenLinkedBodies);

	pub template <typename T>
		T getAs(int vaidationType);

    pub virtual ~PhysicsConstraintConfig();
};

template <typename T>
T PhysicsConstraintConfig::getAs(int vaidationType) {
	if (this->type != vaidationType || rawSubclassPointer == nullptr) {
		throw LogicException(LOC);
	}

	T cVal = static_cast<T>(rawSubclassPointer);

	return cVal;
}

};
