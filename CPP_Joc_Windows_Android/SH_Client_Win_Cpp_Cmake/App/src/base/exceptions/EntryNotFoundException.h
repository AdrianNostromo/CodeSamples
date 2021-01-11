#pragma once

#include <exception>
#include "Exception.h"

struct EntryNotFoundException : public Exception {
public:
    const char * what() const throw () {
        return "EntryNotFoundException";
    }
};
