#pragma once

#include <base/gh.h>
#include <functional>
#include "EL_Base.h"

template <typename T>
class EventListener : public EL_Base {priv typedef EL_Base super;pub dCtor(EventListener);
	pub std::function<void(IEventListener& eventListener, T& param)> cb;
	
	pub explicit EventListener(
		std::function<void(IEventListener& eventListener, T& param)> cb,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected,
		IEventDispatcher* handler);

	pub ~EventListener() override;
};

template <typename T>
EventListener<T>::EventListener(
	std::function<void(IEventListener& eventListener, T& param)> cb,
	std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected,
	IEventDispatcher* handler)
	: super(cb_onHandlerDisconnected, handler),
	cb(cb)
{
	//void
}

template <typename T>
EventListener<T>::~EventListener() {
	//void
}
