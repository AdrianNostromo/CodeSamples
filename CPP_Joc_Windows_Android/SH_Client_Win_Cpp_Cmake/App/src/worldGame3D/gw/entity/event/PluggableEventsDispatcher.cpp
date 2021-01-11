#include "PluggableEventsDispatcher.h"
#include <base/exceptions/LogicException.h>
#include <base/event/listener/EventListener.h>
#include <memory>

using namespace worldGame3D;

PluggableEventsDispatcher::PluggableEventsDispatcher()
	: super()
{
	//void
}

bool PluggableEventsDispatcher::getIsConnected() {
	return gwEventDispatcher != nullptr ? true : false;
}

void PluggableEventsDispatcher::connect(base::TypedEventDispatcher1D<base::GWEvent>* gwEventDispatcher) {
	if (this->gwEventDispatcher != nullptr) {
		throw LogicException(LOC);
	}
	this->gwEventDispatcher = gwEventDispatcher;

	for (ListDL<WrappedListener>::Entry* entry = wrappedListenersList.getFirst(); entry != nullptr; entry = entry->next) {
		WrappedListener& entryData = entry->data;
		
		if (entryData.cb_onHandlerDisconnected) {
			entryData.parentAutoListener = gwEventDispatcher->getDispatcher(entryData.type, true)->addListener(
				entryData.cb,
				nullptr/*autoListenersList*/,
				std::bind(&WrappedListener::onHandlerDisconnected, entryData, std::placeholders::_1)
			);
		} else {
			entryData.parentAutoListener = gwEventDispatcher->getDispatcher(entryData.type, true)->addListener(
				entryData.cb,
				nullptr/*autoListenersList*/,
				nullptr
			);
		}
	}
}

void PluggableEventsDispatcher::disconnect() {
	if (this->gwEventDispatcher == nullptr) {
		throw LogicException(LOC);
	}

	for (ListDL<WrappedListener>::Entry* entry = wrappedListenersList.getFirst(); entry != nullptr; entry = entry->next) {
		WrappedListener& entryData = entry->data;

		entryData.parentAutoListener = nullptr;
	}

	this->gwEventDispatcher = nullptr;
}

std::shared_ptr<IEventListener> PluggableEventsDispatcher::addEventListener(
	int type,
	std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
	std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected)
{
	std::shared_ptr<EventListener<base::GWEvent>> intermediateListener = std::make_shared<EventListener<base::GWEvent>>(cb, cb_onHandlerDisconnected, this);
	ListDL<WrappedListener>::Entry* wrappedListener = wrappedListenersList.appendEmplace(type, cb, intermediateListener.get());
	intermediateListener->inHandler_listEntry = wrappedListener;

	if (gwEventDispatcher != nullptr) {
		if (wrappedListener->data.cb_onHandlerDisconnected) {
			wrappedListener->data.parentAutoListener = gwEventDispatcher->getDispatcher(wrappedListener->data.type, true)->addListener(
				wrappedListener->data.cb,
				nullptr/*autoListenersList*/,
				std::bind(&WrappedListener::onHandlerDisconnected, wrappedListener->data, std::placeholders::_1)
			);
		} else {
			wrappedListener->data.parentAutoListener = gwEventDispatcher->getDispatcher(wrappedListener->data.type, true)->addListener(
				wrappedListener->data.cb,
				nullptr/*autoListenersList*/,
				nullptr
			);
		}
	}

	return intermediateListener;
}

void PluggableEventsDispatcher::addEventListener(
	int type,
	std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList,
	std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected)
{
	std::shared_ptr<IEventListener> intermediateListener = addEventListener(type, cb);
	autoListenersList.appendDirect(intermediateListener);
}

void PluggableEventsDispatcher::addEventListener(
	int type1, int type2,
	std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList,
	std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected)
{
	addEventListener(type1, cb, autoListenersList);
	addEventListener(type2, cb, autoListenersList);
}

void PluggableEventsDispatcher::addEventListener(
	int type1, int type2, int type3,
	std::function<void(IEventListener& eventListener, base::GWEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList,
	std::function<void(IEventListener& eventListener)> cb_onHandlerDisconnected)
{
	addEventListener(type1, cb, autoListenersList);
	addEventListener(type2, cb, autoListenersList);
	addEventListener(type3, cb, autoListenersList);
}

void PluggableEventsDispatcher::removeListener(EL_Base& eventListener) {
	if (eventListener.handler == nullptr) {
		throw LogicException(LOC);
	}

	if (eventListener.cb_onHandlerDisconnected) {
		eventListener.cb_onHandlerDisconnected(eventListener);
	}

	// This is the WrappedListener from wrappedListenersList.
	eventListener.inHandler_listEntry->remove();
	eventListener.inHandler_listEntry = nullptr;
	eventListener.handler = nullptr;

	// At this location, the eventListener memory still exists.
}

void PluggableEventsDispatcher::dispose() {
	while (wrappedListenersList.getFirst() != nullptr) {
		// This must not have any listeners when disposing because it is a special entity buferring dispatcher and is always created.
		throw LogicException(LOC);
	}

	super::dispose();
}

PluggableEventsDispatcher::~PluggableEventsDispatcher() {
	reservedDisposeIfNeeded();// Note1001
}
