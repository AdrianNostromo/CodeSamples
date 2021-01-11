#pragma once

#include <base/gh.h>
#include <base/crashHandler/ICrashHandler.h>

namespace base {
namespace crashHandler {
class CH_Base : public virtual ICrashHandler {pub dCtor(CH_Base);
    pub explicit CH_Base();

    pub void reservedCreate();

    pub virtual void create();

    pub virtual ~CH_Base();
};
};
};
