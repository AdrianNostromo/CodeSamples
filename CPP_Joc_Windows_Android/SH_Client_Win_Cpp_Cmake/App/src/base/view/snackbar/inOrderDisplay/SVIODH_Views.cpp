#include "SVIODH_Views.h"
#include "util/IInOrderView.h"
#include <base/exceptions/LogicException.h>
#include <base/util/AppStyles.h>
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace base;
using namespace base::snackbar;

SVIODH_Views::SVIODH_Views(IApp* app)
	: super(app)
{
	//void
}

void SVIODH_Views::create() {
	super::create();

	app->getLoopDistribution()->addEventListener(
		base::ILoopDistribution::AppLoopEvent_inputDispatch->type,
		std::bind(&SVIODH_Views::onLoopEvent_appLoop_general, this, std::placeholders::_1, std::placeholders::_2),
		localAutoListenersList
	);
}

void SVIODH_Views::addInOrderView(IInOrderView* view, bool doAnimation) {
	if (view->getInDisplayOrder_listEntryRef() != nullptr) {
		throw LogicException(LOC);
	}
	view->getInDisplayOrder_listEntryRef() = viewsList.appendEmplace(view, doAnimation);

	isViewsDirty = true;
}

void SVIODH_Views::removeInOrderView(IInOrderView* view) {
	if (view->getInDisplayOrder_listEntryRef() == nullptr) {
		throw LogicException(LOC);
	}

	view->getInDisplayOrder_listEntryRef()->remove();
	view->getInDisplayOrder_listEntryRef() = nullptr;

	isViewsDirty = true;
	if (viewsList.count() > 0) {
		// Use a small delay until the next view is shown.
		// This seems like a good duration (subjectively chosen, loos fine).
		viewShowDelayS = AppStyles::ANIMATION_COMPLEX_DURATION_S;
	}
}

void SVIODH_Views::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
	if (isViewsDirty || viewShowDelayS > 0.0f) {
		isViewsDirty = false;
		viewShowDelayS -= event.deltaS;

		if (viewsList.count() > 0 && viewShowDelayS <= 0.0f) {
			IInOrderView* view = viewsList.getFirst()->data.view;
			if (!view->getIsViewShowing()) {
				view->showView(viewsList.getFirst()->data.doAnimation);
			}
		}
	}
}

SVIODH_Views::~SVIODH_Views() {
	//void
}
