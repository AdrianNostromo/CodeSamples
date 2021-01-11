#pragma once

#include <worldGame3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <base/exceptions/LogicException.h>

namespace base {
class IGPInteraction {
public:
	virtual IManagedDeviceInput* getInput() = 0;

	template <typename T>
	T getInputAs();

    virtual ~IGPInteraction() = default;
};

template <typename T>
T IGPInteraction::getInputAs() {
	T cVal = dynamic_cast<T>(getInput());
	if(!cVal) {
		throw LogicException(LOC);
	}

	return cVal;
}

};
