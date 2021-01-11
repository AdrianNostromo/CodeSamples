#pragma once

#include <base/gh.h>
#include <base/map/Map2D.h>
#include <memory>
#include "EventDispatcher.h"

namespace base {
template <typename T>
class TypedEventDispatcher2D {pub dCtor(TypedEventDispatcher2D);
	priv Map2D<int, int, std::shared_ptr<EventDispatcher<T>>> eventDispatchersMap{};

	pub explicit TypedEventDispatcher2D();

	pub EventDispatcher<T>* getDispatcher(int typeA, int typeB, bool createIfMissing);

	pub std::shared_ptr<IEventListener> addEventListener(
		int typeA, int typeB,
		std::function<void(IEventListener& eventListener, T& event)> cb);
	pub void addEventListener(
		int typeA, int typeB,
		std::function<void(IEventListener& eventListener, T& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList);
	pub int dispatchEvent(T& event);
	pub int dispatchEvents(ArrayList<T&>* eventsList);

	pub virtual ~TypedEventDispatcher2D();

};

template <typename T>
TypedEventDispatcher2D<T>::TypedEventDispatcher2D() {
	//void
}

template <typename T>
EventDispatcher<T>* TypedEventDispatcher2D<T>::getDispatcher(int typeA, int typeB, bool createIfMissing) {
	std::shared_ptr<EventDispatcher<T>>* eventDispatcher = eventDispatchersMap.getPointer(typeA, typeB);
	if(eventDispatcher == nullptr && createIfMissing) {
		eventDispatcher = eventDispatchersMap.putDirect(typeA, typeB, std::make_shared<EventDispatcher<T>>());
	}
	if (eventDispatcher != nullptr) {
		return eventDispatcher->get();
	}

	return nullptr;
}

template <typename T>
std::shared_ptr<IEventListener> TypedEventDispatcher2D<T>::addEventListener(
	int typeA, int typeB,
	std::function<void(IEventListener& eventListener, T& event)> cb)
{
	return getDispatcher(typeA, typeB, true)->addListener(cb);
}

template <typename T>
void TypedEventDispatcher2D<T>::addEventListener(
	int typeA, int typeB,
	std::function<void(IEventListener& eventListener, T& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	getDispatcher(typeA, typeB, true)->addListener(cb, &autoListenersList);
}

template <typename T>
int TypedEventDispatcher2D<T>::dispatchEvent(T& event) {
	int ct = 0;

	base::EventDispatcher<T>* dis = getDispatcher(event->getTypeA(), event->getTypeB(), false);
	if (dis != nullptr) {
		ct += dis->dispatch(event);
	}

	event->clearData();

	return ct;
}

template <typename T>
int TypedEventDispatcher2D<T>::dispatchEvents(ArrayList<T&>* eventsList) {
	int ct = 0;
	if (eventsList != nullptr) {
		for (int i = 0; i < eventsList->count(); i++) {
			T& event = eventsList->getReference(i);

			ct += dispatchEvent(event);
		}
	}

	return ct;
}

template <typename T>
TypedEventDispatcher2D<T>::~TypedEventDispatcher2D() {
	//void
}

};
