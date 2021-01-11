#pragma once

#include <exception>
#include "Exception.h"

struct NotDirectoryException : public Exception {
public:
    const char * what() const throw () {
        return "NotDirectoryException";
    }
};
