#pragma once

#include <exception>
#include "Exception.h"

struct BoundsException : public Exception {
public:
    const char * what() const throw () {
        return "BoundsException";
    }
};
