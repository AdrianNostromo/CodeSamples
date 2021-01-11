#pragma once

#include <base/gh.h>
#include "IObject.h"

namespace base {
class IManagedObject : virtual public IObject {
    pub virtual bool getIsCreated() = 0;

    pub virtual void reservedCreate() = 0;

    pub ~IManagedObject() override = default;
};
};
