#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>

namespace rpg3D {
class MovementConfig {pub dCtor(MovementConfig);
    prot static int NewType();

    priv int type;

    // This is used to avoid dynamic cast.
    // It contains the subclass void*;
    prot void* rawSubclassPointer = nullptr;

    pub explicit MovementConfig(int type);

    pub int getType();

    pub template <typename T>
    T castAs(int validationType);

    pub virtual ~MovementConfig();
};

template <typename T>
T MovementConfig::castAs(int validationType) {
    if (this->type != validationType || rawSubclassPointer == nullptr) {
        throw LogicException(LOC);
    }

    T cVal = static_cast<T>(rawSubclassPointer);

    return cVal;
}

};
