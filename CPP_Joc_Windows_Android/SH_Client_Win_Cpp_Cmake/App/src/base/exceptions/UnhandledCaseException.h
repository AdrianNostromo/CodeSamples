#pragma once

#include <exception>
#include "Exception.h"

struct UnhandledCaseException : public Exception {
public:
    UnhandledCaseException()
        : Exception()
    {
        //void
    }

    const char * what() const throw () {
        return "UnhandledCaseException";
    }
};
