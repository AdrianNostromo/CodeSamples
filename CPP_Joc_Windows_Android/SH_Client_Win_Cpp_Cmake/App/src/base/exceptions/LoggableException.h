#pragma once

#include <base/gh.h>
#include "Exception.h"

struct LoggableException : public Exception {priv typedef Exception super;
    pub explicit LoggableException(const char* exceptionType, const char* fileName, size_t lineIndex);

};
