#pragma once

#include <base/gh.h>
#include "LoggableException.h"
#include <base/DevUtils.h>

struct LogicException : public LoggableException {priv typedef LoggableException super;
    pub explicit LogicException(const char* fileName, size_t lineIndex)
        : LoggableException("LogicException", fileName, lineIndex)
    {
        //void
    }

    pub const char* what() const throw () {
        return "LogicException";
    }
};
