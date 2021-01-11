#pragma once

#include <exception>
#include "Exception.h"
#include "ResourceNotFoundException.h"

struct FileNotFoundException : public ResourceNotFoundException {
private: typedef ResourceNotFoundException super;
public:
    FileNotFoundException()
        : super()
    {
        //void
    }

    const char * what() const throw () {
        return "FileNotFoundException";
    }
};
