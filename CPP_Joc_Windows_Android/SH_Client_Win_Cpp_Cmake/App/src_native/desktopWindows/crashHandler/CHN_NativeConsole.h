#pragma once

#include <base/gh.h>
#include "CHN_Base.h"

namespace desktopWindows {
class CHN_NativeConsole : public CHN_Base {priv typedef CHN_Base super;pub dCtor(CHN_NativeConsole);
    pub explicit CHN_NativeConsole();

    prot void consoleOutputErrorLogs() final;

    pub ~CHN_NativeConsole() override;
};
};
