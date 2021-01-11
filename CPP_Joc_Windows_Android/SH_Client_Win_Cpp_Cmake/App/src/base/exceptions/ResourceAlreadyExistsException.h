#pragma once

#include <exception>
#include "Exception.h"

struct ResourceAlreadyExistsException : public Exception {
public:
	ResourceAlreadyExistsException()
		: Exception()
	{
		//void
	}

	const char * what() const throw () {
		return "ResourceAlreadyExistsException";
	}
};
