#include "UIV_AutoDisposable.h"

using namespace base;

UIV_AutoDisposable::UIV_AutoDisposable(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
	: super(app, viewItemConfigs, viewEaseDurationsSList),
	handlerAutoDisposal(handlerAutoDisposal)
{
	//void
}

void UIV_AutoDisposable::animationFinishedEvent() {
	super::animationFinishedEvent();

	if (!isActive()) {
		isReadyForAutoDisposal = true;
	}
}

void UIV_AutoDisposable::onVisualLoopsFinished() {
	if (isReadyForAutoDisposal) {
		if (handlerAutoDisposal != nullptr) {
			//asd_x;// Note. This will cause this view memory to be deleted. Not the best solution but works for now.
			/// The problem that keeps from implementing a better version is the loop distribution through BasicTreePart, afte that code is removed, implement a better solution.
			handlerAutoDisposal->onRequestDisposeView(this);
		}
	}
}

UIV_AutoDisposable::~UIV_AutoDisposable() {
	//void
}
