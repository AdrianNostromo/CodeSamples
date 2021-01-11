#include "EL_Base.h"
#include <base/event/dispatcher/IEventDispatcher.h>

EL_Base::EL_Base(
	std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected,
	IEventDispatcher* handler)
	: cb_onHandlerDisconnected(cb_onHandlerDisconnected),
	handler(handler)
{
	//void
}

bool EL_Base::isConnected() {
	if (handler != nullptr) {
		return true;
	}

	return false;
}

void EL_Base::disconnect() {
	if (handler == nullptr) {
		return;
	}

	handler->removeListener(*this);
}

EL_Base::~EL_Base() {
	if (handler != nullptr) {
		disconnect();
	}
}
