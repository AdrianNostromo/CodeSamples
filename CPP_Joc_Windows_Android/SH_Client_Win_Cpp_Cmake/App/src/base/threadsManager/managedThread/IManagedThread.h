#pragma once

#include <base/gh.h>
#include <base/object/IObject.h>

namespace base {
class IManagedThread : virtual public IObject {
    // This will dispose the thread when available.
    pub virtual void removeManaged() = 0;

    pub virtual ~IManagedThread() = default;
};
};
