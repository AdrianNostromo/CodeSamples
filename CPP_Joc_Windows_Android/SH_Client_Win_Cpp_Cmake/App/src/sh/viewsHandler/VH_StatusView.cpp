#include "VH_StatusView.h"
#include <sh/view/status/StatusView.h>
#include <base/appLoop/ILoopDistribution.h>
#include <sh/app/IAppSquareHeads.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>
#include <graphics/visual2d/drawable/IContainer2D.h>

using namespace sh;

VH_StatusView::VH_StatusView(IAppSquareHeads* app)
	: super(app)
{
	//void
}

void VH_StatusView::create() {
	super::create();

	app->getLoopDistribution()->addEventListener(
		base::ILoopDistribution::AppLoopEvent_gameWorld_pre->type,
		std::bind(&VH_StatusView::onLoopEvent_appLoop_General, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

IStatusView* VH_StatusView::getOrCreateStatusView() {
	if (statusView == nullptr) {
		addComponent(statusView = new StatusView(app), true/*doCreate*/);
		layerStatusView->addChild(statusView->getVisualsHolder());
		statusView->show(false);
	}

	return statusView;
}

void VH_StatusView::onLoopEvent_appLoop_General(IEventListener& eventListener, base::LoopEvent& event) {
	if (statusView != nullptr && !statusView->getIsViewActive()) {
		float iddleTimerS = statusView->getInactiveViewIdleTimerS() + event.deltaS;
		statusView->setInactiveViewIdleTimerS(iddleTimerS);

		if (iddleTimerS >= 0.5f) {
			statusView->hide(false);

			removeComponent(statusView, true/*doDispose*/);
			statusView = nullptr;
		}
	}
}

VH_StatusView::~VH_StatusView() {
	//void
}
