#pragma once

#include <exception>
#include "Exception.h"

struct DataFormatException : public Exception {
public:
	DataFormatException()
		: Exception()
	{
		//void
	}
	
    const char * what() const throw () {
        return "DataFormatException";
    }
};
