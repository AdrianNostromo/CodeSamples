#include "ManagedListener.h"
#include <cassert>
#include <base/exceptions/LogicException.h>

using namespace base;

ManagedListener::ManagedListener() {
	//void
}

bool ManagedListener::getIsListening() {
	return isListening;
}

void ManagedListener::setIsListening(bool isListening) {
	if(this->isListening == isListening) {
		throw LogicException(LOC);
	}

	this->isListening = isListening;
}

ManagedListener::~ManagedListener() {
	if(isListening) {
		// Lister was not removed before deletion.
		assert(false);
	}
}
