#pragma once

#include <base/gh.h>
#include "IEventListener.h"
#include <functional>

class IEventDispatcher;
class IListEntry;

class EL_Base : virtual public IEventListener {pub dCtor(EL_Base);
	// If this is used, the events_dispatcher may be removed while it still has this listened.
	// When this is nullptr, this listener must be disconnected before the dispatcher is disposed or an error is triggerred.
	pub std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected;

	pub IEventDispatcher* handler;
	pub IListEntry* inHandler_listEntry = nullptr;

    pub explicit EL_Base(
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected,
		IEventDispatcher* handler);

	pub bool isConnected() final;
	pub void disconnect() final;

    pub ~EL_Base() override;
};
