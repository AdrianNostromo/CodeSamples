#pragma once

#include <base/gh.h>
#include <string>

namespace base {
class ICrashHandler {
    pub static ICrashHandler* crashHandler;

    pub virtual void pushEntry_Info(const char* fileName, int lineIndex, std::string msg) = 0;
    pub virtual void pushEntry_CriticalException(const char* fileName, int lineIndex, std::string msg) = 0;

    pub virtual void handleCrash() = 0;

    pub virtual ~ICrashHandler() = default;
};
};
