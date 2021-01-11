#pragma once

#include <base/gh.h>
#include "CHN_NativeConsole.h"

namespace desktopWindows {
class CHN_FileSystem : public CHN_NativeConsole {priv typedef CHN_NativeConsole super;pub dCtor(CHN_FileSystem);
    pub explicit CHN_FileSystem();

    prot void writeCrashLogs() final;

    pub ~CHN_FileSystem() override;
};
};
