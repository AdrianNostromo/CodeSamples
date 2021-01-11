#pragma once

#include <exception>
#include "Exception.h"

struct NotFileException : public Exception {
public:
    const char * what() const throw () {
        return "NotFileException";
    }
};
