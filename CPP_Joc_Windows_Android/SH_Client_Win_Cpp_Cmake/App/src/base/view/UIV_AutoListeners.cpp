#include "UIV_AutoListeners.h"
#include <base/view/listener/IAutoListener.h>

using namespace base;

UIV_AutoListeners::UIV_AutoListeners(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
	: super(app, viewItemConfigs, viewEaseDurationsSList)
{
	//void
}

void UIV_AutoListeners::addListener(IAutoListener* listener) {
	if (listener->getDispatcher() != nullptr || listenersList.contains(listener)) {
		throw LogicException(LOC);
	};

	listener->setDispatcher(this);
	listenersList.appendDirect(listener);
}

void UIV_AutoListeners::removeListener(IAutoListener* listener) {
	if (listener->getDispatcher() != this || !listenersList.remove(listener)) {
		throw LogicException(LOC);
	}

	listener->setDispatcher(nullptr);
}

void UIV_AutoListeners::onAutoListenerDisposing(IAutoListener* listener) {
	removeListener(listener);
}

void UIV_AutoListeners::disposeMain() {
	while (listenersList.count() > 0) {
		IAutoListener* listener = listenersList.getDirect(listenersList.count() - 1);

		removeListener(listener);
	}

	super::disposeMain();
}

UIV_AutoListeners::~UIV_AutoListeners() {
	//void
}
