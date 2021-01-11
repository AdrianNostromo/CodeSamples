#pragma once

#include <exception>
#include "Exception.h"

struct ClassCastException : public Exception {
public:
    const char * what() const throw () {
        return "ClassCastException";
    }
};
