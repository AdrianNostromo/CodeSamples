#pragma once

#include <base/gh.h>
#include "CH_Entries.h"

namespace base {
namespace crashHandler {
class CH_HandleCrash : public CH_Entries {priv typedef CH_Entries super;pub dCtor(CH_HandleCrash);
    pub explicit CH_HandleCrash();

    pub void handleCrash() final;

    prot virtual void writeCrashLogs() = 0;
    prot virtual void consoleOutputErrorLogs() = 0;

    pub ~CH_HandleCrash() override;
};
};
};
