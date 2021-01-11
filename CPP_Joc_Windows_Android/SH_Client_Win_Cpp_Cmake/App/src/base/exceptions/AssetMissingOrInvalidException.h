#pragma once

#include <exception>
#include "Exception.h"

struct AssetMissingOrInvalidException : public Exception {
public:
    AssetMissingOrInvalidException()
        : Exception()
    {
        //void
    }

    const char * what() const throw () {
        return "AssetMissingOrInvalidException";
    }
};
