#pragma once

#include <exception>
#include "Exception.h"

struct ValueNotFoundException : public Exception {
public:
    ValueNotFoundException()
        : Exception()
    {
        //void
    }

    const char * what() const throw () {
        return "ValueNotFoundException";
    }
};
