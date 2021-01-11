#pragma once

#include <base/gh.h>
#include "IDisposable.h"
#include <base/exceptions/LogicException.h>

namespace base {
class IObject : public virtual IDisposable {
	pub template <typename T>
	T castAs();

	pub template <typename T>
	T castAsOptional();

	pub ~IObject() override = default;
};

template <typename T>
T IObject::castAs() {
	T cVal = dynamic_cast<T>(this);
	if(!cVal) {
		throw LogicException(LOC);
	}

	return cVal;
}

template <typename T>
T IObject::castAsOptional() {
	T cVal = dynamic_cast<T>(this);
	if(!cVal) {
		return nullptr;
	}

	return cVal;
}

};
