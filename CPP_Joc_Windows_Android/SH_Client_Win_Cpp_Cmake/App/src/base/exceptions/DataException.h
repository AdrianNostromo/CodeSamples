#pragma once

#include <exception>
#include "Exception.h"

struct DataException : public Exception {
public:
	DataException()
		: Exception()
	{
		//void
	}

    const char * what() const throw () {
        return "DataException";
    }
};
