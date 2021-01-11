#pragma once

#include <exception>
#include "Exception.h"

struct NotImplementedException : public Exception {
public:
    NotImplementedException()
        : Exception()
    {
        //void
    }

    const char * what() const throw () {
        return "NotImplementedException";
    }
};
