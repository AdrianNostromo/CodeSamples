#pragma once

#include <base/gh.h>
#include <base/DevUtils.h>
#include <memory>
#include <string>
#include <base/exceptions/LogicException.h>

namespace rpg3D {
class EntityConfigBase {pub dCtor(EntityConfigBase);
    prot static int NewType();

    prot int type;

    pub std::shared_ptr<std::string> instanceId;

    prot void* rawSubclassPointer = nullptr;

    pub explicit EntityConfigBase(int type, std::shared_ptr<std::string> instanceId);

    pub int getType();

    pub template <typename T>
    T castAs(int validationType);

    pub virtual ~EntityConfigBase();
};

template <typename T>
T EntityConfigBase::castAs(int validationType) {
    if (this->type != validationType || rawSubclassPointer == nullptr) {
        throw LogicException(LOC);
    }

    T cVal = static_cast<T>(rawSubclassPointer);

    return cVal;
}

};
