#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>

namespace physics3D {
class PhysicsShapeConfig {pub dCtor(PhysicsShapeConfig);
	prot static int NewType();
	
	pub const int type;

	// This is used to avoid dynamic cast.
	// It contains the subclass void*;
	prot void* rawSubclassPointer = nullptr;

    pub explicit PhysicsShapeConfig(int type);

	pub template <typename T>
		T getAs(int vaidationType);

    pub virtual ~PhysicsShapeConfig();
};

template <typename T>
T PhysicsShapeConfig::getAs(int vaidationType) {
	if (this->type != vaidationType || rawSubclassPointer == nullptr) {
		throw LogicException(LOC);
	}

	T cVal = static_cast<T>(rawSubclassPointer);

	return cVal;
}

};
