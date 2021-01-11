#pragma once

#include <base/gh.h>
#include <base/console/INativeConsole.h>

namespace desktopWindows {
class NativeConsole : public virtual base::INativeConsole {priv typedef base::INativeConsole super;
    pub explicit NativeConsole();

    pub void post_verbose(const char* fmt, va_list ap) override;
    pub void post_debug(const char* fmt, va_list ap) override;
    pub void post_info(const char* fmt, va_list ap) override;
    pub void post_warn(const char* fmt, va_list ap) override;
    pub void post_error(const char* fmt, va_list ap) override;

    pub ~NativeConsole() override;
};
};
