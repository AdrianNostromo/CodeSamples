#pragma once

#include <exception>
#include "Exception.h"

struct NotFoundException : public Exception {
public:
	const char * what() const throw () {
		return "NotFoundException";
	}
};
