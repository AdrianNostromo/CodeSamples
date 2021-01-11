#include "UIV_Events.h"
#include <base/view/listener/IAutoListener.h>

using namespace base;

UIV_Events::UIV_Events(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
	: super(app, viewItemConfigs, viewEaseDurationsSList)
{
	//void
}

base::TypedEventDispatcher1D<ViewEvent>& UIV_Events::getEvents() {
	return events;
}

void UIV_Events::disposeMain() {
	//void

	super::disposeMain();
}

UIV_Events::~UIV_Events() {
	//void
}
