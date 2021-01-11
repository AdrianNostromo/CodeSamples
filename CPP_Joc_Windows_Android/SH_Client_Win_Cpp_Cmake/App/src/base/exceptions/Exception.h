#pragma once

#include <exception>

struct Exception : public std::exception {
public:
    Exception()
        : std::exception()
    {
        //void
    }

    const char * what() const throw () {
        return "Exception";
    }
};
