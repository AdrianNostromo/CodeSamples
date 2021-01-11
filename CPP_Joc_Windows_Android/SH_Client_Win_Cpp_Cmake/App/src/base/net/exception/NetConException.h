#pragma once

#include <exception>
#include <base/exceptions/Exception.h>

struct NetConException : public Exception {
public:
    const char * what() const throw () {
        return "NetConException";
    }
};
