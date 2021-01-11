#pragma once

#include <exception>
#include "Exception.h"

struct IOException : public Exception {
public:
    IOException()
        : Exception()
    {
        //void
    }

    const char * what() const throw () {
        return "IOException";
    }
};
