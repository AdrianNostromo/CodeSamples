#pragma once

#include <base/gh.h>
#include <stdarg.h>

namespace base {
class INativeConsole {
    pub virtual void post_verbose(const char* fmt, va_list ap) = 0;
    pub virtual void post_debug(const char* fmt, va_list ap) = 0;
    pub virtual void post_info(const char* fmt, va_list ap) = 0;
    pub virtual void post_warn(const char* fmt, va_list ap) = 0;
    pub virtual void post_error(const char* fmt, va_list ap) = 0;

    pub virtual ~INativeConsole() = default;
};
};
