#pragma once

#include <base/gh.h>
#include <base/map/Map1D.h>
#include <memory>
#include "EventDispatcher.h"

namespace base {
template <typename T>
class TypedEventDispatcher1D {pub dCtor(TypedEventDispatcher1D);
	priv Map1D<int, std::shared_ptr<EventDispatcher<T>>> eventDispatchersMap{};

	pub explicit TypedEventDispatcher1D();

	pub EventDispatcher<T>* getDispatcher(int type, bool createIfMissing);

	pub std::shared_ptr<IEventListener> addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, T& event)> cb);
	pub void addEventListener(
		int type,
		std::function<void(IEventListener& eventListener, T& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList);
	pub void addEventListener(
		int type1, int type2,
		std::function<void(IEventListener& eventListener, T& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList);
	pub void addEventListener(
		int type1, int type2, int type3,
		std::function<void(IEventListener& eventListener, T& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList);
	pub int dispatchEvent(T& event);
	pub int dispatchEvents(ArrayList<T&>* eventsList);

	pub virtual ~TypedEventDispatcher1D();

};

template <typename T>
TypedEventDispatcher1D<T>::TypedEventDispatcher1D() {
	//void
}

template <typename T>
EventDispatcher<T>* TypedEventDispatcher1D<T>::getDispatcher(int type, bool createIfMissing) {
	std::shared_ptr<EventDispatcher<T>>* eventDispatcher = eventDispatchersMap.getPointer(type);
	if(eventDispatcher == nullptr && createIfMissing) {
        eventDispatcher = eventDispatchersMap.putDirect(type, std::make_shared<EventDispatcher<T>>());
	}
    if (eventDispatcher != nullptr) {
        return eventDispatcher->get();
    }

	return nullptr;
}

template <typename T>
std::shared_ptr<IEventListener> TypedEventDispatcher1D<T>::addEventListener(
    int type,
    std::function<void(IEventListener& eventListener, T& event)> cb)
{
    return getDispatcher(type, true)->addListener(cb);
}

template <typename T>
void TypedEventDispatcher1D<T>::addEventListener(
    int type,
    std::function<void(IEventListener& eventListener, T& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    getDispatcher(type, true)->addListener(cb, &autoListenersList);
}

template <typename T>
void TypedEventDispatcher1D<T>::addEventListener(
    int type1, int type2,
    std::function<void(IEventListener& eventListener, T& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    addEventListener(type1, cb, autoListenersList);
    addEventListener(type2, cb, autoListenersList);
}

template <typename T>
void TypedEventDispatcher1D<T>::addEventListener(
    int type1, int type2, int type3,
    std::function<void(IEventListener& eventListener, T& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    addEventListener(type1, cb, autoListenersList);
    addEventListener(type2, cb, autoListenersList);
    addEventListener(type3, cb, autoListenersList);
}

template <typename T>
int TypedEventDispatcher1D<T>::dispatchEvent(T& event) {
    int ct = 0;

    base::EventDispatcher<T>* dis = getDispatcher(event.type, false);
    if (dis != nullptr) {
        ct += dis->dispatch(event);
    }

    event.clearData();

    return ct;
}

template <typename T>
int TypedEventDispatcher1D<T>::dispatchEvents(ArrayList<T&>* eventsList) {
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
TypedEventDispatcher1D<T>::~TypedEventDispatcher1D() {
	//void
}

};
