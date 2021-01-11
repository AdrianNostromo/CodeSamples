#pragma once

#include <exception>
#include "Exception.h"

struct ResourceNotFoundException : public Exception {
public:
	ResourceNotFoundException()
		: Exception()
	{
		//void
	}

	const char * what() const throw () {
		return "ResourceNotFoundException";
	}
};
