#pragma once

#include <exception>
#include "Exception.h"

struct AlreadyExistsException : public Exception {
public:
	AlreadyExistsException()
		: Exception()
	{
		//void
	}

	const char * what() const throw () {
		return "AlreadyExistsException";
	}
};
