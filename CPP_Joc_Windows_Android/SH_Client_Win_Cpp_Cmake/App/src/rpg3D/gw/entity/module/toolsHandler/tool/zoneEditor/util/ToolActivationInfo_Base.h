#pragma once

#include <base/gh.h>
#include <base/exceptions/LogicException.h>

namespace rpg3D {
class ToolActivationInfo_Base {pub dCtor(ToolActivationInfo_Base);
    prot static int NewType();

    prot int type;

    pub int cursorIndex;

    prot void* rawSubclassPointer = nullptr;

    pub explicit ToolActivationInfo_Base(int type, int cursorIndex);

    pub int getType();

    // This is extended and used to clear sensitised cursor delta after processing.
    pub virtual void clearCumulators();
    //asd_01;// This may be needed to avoid inputs dropping if on the same frame.
    //pub virtual void addCumulators(ToolActivationInfo_Base* other);

    pub template <typename T>
    T castAs(int validationType);

    pub virtual ~ToolActivationInfo_Base();
};

template <typename T>
T ToolActivationInfo_Base::castAs(int validationType) {
    if (this->type != validationType || rawSubclassPointer == nullptr) {
        throw LogicException(LOC);
    }

    T cVal = static_cast<T>(rawSubclassPointer);

    return cVal;
}

};
