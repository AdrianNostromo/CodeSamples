#include "LD_Listeners.h"

using namespace base;

LD_Listeners::LD_Listeners()
	: super()
{
	//void
}

base::TypedEventDispatcher1D<base::LoopEvent>* LD_Listeners::getEventDispatcher() {
	return &eventDispatcher;
}

std::shared_ptr<IEventListener> LD_Listeners::addEventListener(
	int type,
	std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb)
{
	return eventDispatcher.getDispatcher(type, true)->addListener(cb);
}

void LD_Listeners::addEventListener(
	int type,
	std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	base::EventDispatcher<base::LoopEvent>* dispatcher = eventDispatcher.getDispatcher(type, true);
	dispatcher->addListener(cb, &autoListenersList);
}

void LD_Listeners::addEventListener(
	int type1, int type2,
	std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	addEventListener(type1, cb, autoListenersList);
	addEventListener(type2, cb, autoListenersList);
}

void LD_Listeners::addEventListener(
	int type1, int type2, int type3,
	std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	addEventListener(type1, cb, autoListenersList);
	addEventListener(type2, cb, autoListenersList);
	addEventListener(type3, cb, autoListenersList);
}

void LD_Listeners::addEventListener(
	ArrayList<int>& eventTypesList,
	std::function<void(IEventListener& eventListener, base::LoopEvent& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	for (int i = 0; i < eventTypesList.count(); i++) {
		int type = eventTypesList.getDirect(i);

		addEventListener(type, cb, autoListenersList);
	}
}

int LD_Listeners::dispatchEvent(base::LoopEvent& event) {
	int ct = 0;

	base::EventDispatcher<base::LoopEvent>* d = eventDispatcher.getDispatcher(event.type, false);
	if (d != nullptr) {
		ct += d->dispatch(event);
	}

	event.clearData();

	return ct;
}

LD_Listeners::~LD_Listeners() {
	//void
}
