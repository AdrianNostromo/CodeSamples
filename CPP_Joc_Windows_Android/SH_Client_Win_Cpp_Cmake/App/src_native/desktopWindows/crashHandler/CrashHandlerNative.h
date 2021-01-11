#pragma once

#include <base/gh.h>
#include "CHN_FileSystem.h"

namespace desktopWindows {
class CrashHandlerNative : public CHN_FileSystem {priv typedef CHN_FileSystem super;pub dCtor(CrashHandlerNative);
    pub explicit CrashHandlerNative();

    pub ~CrashHandlerNative() override;
};
};
