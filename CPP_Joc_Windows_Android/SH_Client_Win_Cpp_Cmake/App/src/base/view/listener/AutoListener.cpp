#include "AutoListener.h"
#include "IDispatcherAutoListener.h"

using namespace base;

AutoListener::AutoListener() {
	//void
}

IDispatcherAutoListener* AutoListener::getDispatcher() {
	return dispatcher;
}

void AutoListener::setDispatcher(IDispatcherAutoListener* dispatcher) {
	this->dispatcher = dispatcher;
}

AutoListener::~AutoListener() {
	if (dispatcher != nullptr) {
		dispatcher->onAutoListenerDisposing(this);
	}
}
