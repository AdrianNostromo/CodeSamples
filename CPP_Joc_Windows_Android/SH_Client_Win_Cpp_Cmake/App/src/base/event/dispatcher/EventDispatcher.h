#pragma once

#include <base/gh.h>
#include <base/object/Object.h>
#include <functional>
#include <base/list/ListDL.h>
#include <base/event/listener/EventListener.h>
#include <memory>
#include <base/list/ArrayList.h>
#include "IEventDispatcher.h"
#include <base/exceptions/LogicException.h>
#include <base/list/IListEntry.h>
#include <base/MM.h>

namespace base {
template <typename T>
class EventDispatcher final/*Note1001. calls_the_reserved_dispose*/ : public base::Object, public virtual IEventDispatcher {priv typedef base::Object super;pub dCtor(EventDispatcher);
	// The EventListener* is from a s_p and the s_p will make sure to call the local removeListener(...) before the memory is released.
	/// That ensures the pointers are always valid.
	priv ListDL<EventListener<T>*> cbList{LOC};

    pub explicit EventDispatcher();

	pub ListDL<EventListener<T>*>* peekCbList();
	pub int getListenersCount();

	pub std::shared_ptr<EventListener<T>> addListener(
		std::function<void(IEventListener& eventListener, T& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>* autoListenersList=nullptr,
		std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected=nullptr,
		void* extraData = nullptr);

	pub void removeListener(EL_Base& eventListener) final;

	pub int dispatch(T& param);

	prot void dispose() override;
	pub ~EventDispatcher() override;
};

template <typename T>
EventDispatcher<T>::EventDispatcher()
	: super()
{
	//void
}

template <typename T>
ListDL<EventListener<T>*>* EventDispatcher<T>::peekCbList() {
	return &cbList;
}

template <typename T>
int EventDispatcher<T>::getListenersCount() {
	return cbList.count();
}

template <typename T>
std::shared_ptr<EventListener<T>> EventDispatcher<T>::addListener(
	std::function<void(IEventListener& eventListener, T& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>* autoListenersList,
	std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected,
	void* extraData)
{
	std::shared_ptr<EventListener<T>> listener = std::make_shared<EventListener<T>>(cb, cb_onHandlerDisconnected, this);
	
	typename ListDL<EventListener<T>*>::Entry* listEntry = cbList.appendDirect(listener.get());
	listEntry->extraData = extraData;
	listener->inHandler_listEntry = listEntry;

	if (autoListenersList != nullptr) {
		autoListenersList->appendDirect(listener);
	}

	return listener;
}

template <typename T>
void EventDispatcher<T>::removeListener(EL_Base& eventListener) {
	if (eventListener.handler == nullptr) {
		throw LogicException(LOC);
	}

	if (eventListener.cb_onHandlerDisconnected) {
		eventListener.cb_onHandlerDisconnected(eventListener);
	}
	
	eventListener.inHandler_listEntry->remove();
	eventListener.inHandler_listEntry = nullptr;
	eventListener.handler = nullptr;

	// At this location, the eventListener memory still exists.
}

template <typename T>
int EventDispatcher<T>::dispatch(T& param) {
	int ct = 0;

	cbList.nextEnumeratingEntry = cbList.getFirst();
	cbList.isEnumeratingInProgress = true;

	typename ListDL<EventListener<T>*>::Entry* listEntry;
	while((listEntry = cbList.nextEnumeratingEntry) != nullptr) {
		cbList.nextEnumeratingEntry = cbList.nextEnumeratingEntry->next;

		EventListener<T>* listener = listEntry->data;
		listener->cb(*listener, param);
		ct++;
	}

	cbList.isEnumeratingInProgress = false;

	return ct;
}

template <typename T>
void EventDispatcher<T>::dispose() {
	while (cbList.getFirst() != nullptr) {
		if (cbList.getFirst()->data->cb_onHandlerDisconnected) {
			// The listeners that don't use cb_onHandlerDisconnected must disconnect before this gets disposed.
			throw LogicException(LOC);
		}

		// The listener has a cb_onHandlerDisconnected and so it supports dispatcher dispose while the listener is active.

		removeListener(*(cbList.getFirst()->data));
	}

	super::dispose();
}

template <typename T>
EventDispatcher<T>::~EventDispatcher() {
	reservedDisposeIfNeeded();// Note1001
}

};
