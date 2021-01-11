#include "ViewEvent.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

int ViewEvent::typeCounter = 0;

ViewEvent::ViewEvent(std::string name)
	: name(std::move(name))
{
	type = ++typeCounter;
}

void ViewEvent::clearData() {
	view = nullptr;

	userData_voidP = nullptr;
}

ViewEvent* ViewEvent::setUserData_voidP(
	IUIView* view, void* userData_voidP)
{
	this->view = view;

	this->userData_voidP = userData_voidP;

	return this;
}

ViewEvent::~ViewEvent() {
	//void
}
