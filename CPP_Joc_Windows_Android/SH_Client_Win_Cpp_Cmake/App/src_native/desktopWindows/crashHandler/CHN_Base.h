#pragma once

#include <base/gh.h>
#include <base/crashHandler/CrashHandler.h>

namespace desktopWindows {
class CHN_Base : public base::CrashHandler {priv typedef base::CrashHandler super;pub dCtor(CHN_Base);
    pub explicit CHN_Base();

    pub ~CHN_Base() override;
};
};
