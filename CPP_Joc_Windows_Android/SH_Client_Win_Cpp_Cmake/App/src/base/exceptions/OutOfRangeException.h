#pragma once

#include <exception>
#include "Exception.h"

struct OutOfRangeException : public Exception {
public:
	OutOfRangeException()
		: Exception()
	{
		//void
	}

	const char * what() const throw () {
		return "OutOfRangeException";
	}
};
