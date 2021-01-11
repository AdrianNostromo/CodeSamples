#pragma once

#include <base/gh.h>
#include "CH_HandleCrash.h"

namespace base {
class CrashHandler : public crashHandler::CH_HandleCrash {priv typedef crashHandler::CH_HandleCrash super;pub dCtor(CrashHandler);
    pub explicit CrashHandler();

    pub ~CrashHandler() override;
};
};
