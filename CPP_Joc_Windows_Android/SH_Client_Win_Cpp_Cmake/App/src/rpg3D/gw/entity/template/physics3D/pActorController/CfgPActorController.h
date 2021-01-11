#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>

namespace rpg3D {
class CfgPActorController {pub dCtor(CfgPActorController);
	prot static int GetNew_TYPE();

	priv int type;

	prot void* rawTopSubclassPointer = nullptr;

    pub explicit CfgPActorController(int type);
	
	pub int getType();

    // Note. Don't include data_type check because the same data_class is used for multiple data_types.
    pub template <typename T>
    T castToSubtype(int validationType);

    pub virtual ~CfgPActorController();
};

template <typename T>
T CfgPActorController::castToSubtype(int validationType) {
	if (rawTopSubclassPointer == nullptr || type != validationType) {
		throw LogicException(LOC);
	}

	T cVal = static_cast<T>(rawTopSubclassPointer);

	return cVal;
}

};
