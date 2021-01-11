#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>

namespace rpg3D {
class CfgConstraint3D {pub dCtor(CfgConstraint3D);
    prot static int NewType();
	
	pub const int type;

	prot void* rawSubclassPointer = nullptr;

	pub explicit CfgConstraint3D(int type);

	pub template <typename T>
		T getAs(int vaidationType);

    pub virtual ~CfgConstraint3D();
};

template <typename T>
T CfgConstraint3D::getAs(int vaidationType) {
	if (this->type != vaidationType || rawSubclassPointer == nullptr) {
		throw LogicException(LOC);
	}

	T cVal = static_cast<T>(rawSubclassPointer);

	return cVal;
}

};
