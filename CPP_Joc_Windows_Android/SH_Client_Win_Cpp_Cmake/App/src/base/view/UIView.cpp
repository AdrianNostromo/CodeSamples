#include "UIView.h"
#include <base/exceptions/LogicException.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/event/listener/IEventListener.h>
#include <base/appLoop/event/LoopEvent.h>
#include <base/app/IApp.h>

std::string UIView::VFLAG_in = "_in_";
std::string UIView::VFLAG_out = "_out_";

UIView::UIView(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, IHandlerAutoDisposal* handlerAutoDisposal)
    : super(app, viewItemConfigs, viewEaseDurationsSList, handlerAutoDisposal)
{
    //void
}

void UIView::create() {
    super::create();

    app->getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_ui->type,
        std::bind(&UIView::onLoopEvent_appLoop_UI, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );
}

void UIView::onLoopEvent_appLoop_UI(IEventListener& eventListener, base::LoopEvent& event) {
    isAnyAppLoopCalled = true;

    visualLoop(event.deltaS, event.deltaMS);
    visualLoopB(event.deltaS, event.deltaMS);

    //asd_x;// Note. This will cause this view memory to be deleted. Not the best solution but works for now.
    /// The problem that keeps from implementing a better version is the loop distribution through BasicTreePart, afte that code is removed, implement a better solution.
    onVisualLoopsFinished();
}

void UIView::show(bool doAnimation, ViewDataBase* viewData, IAnimationListener* menuAnimationListener) {
    if (isActive()) {
		return;
    }

    this->viewData = viewData;

    onShowRequested_pre();

    std::string stateID = computeStateFlags("_in_");
    if (!doAnimation || endStateID != stateID) {
        setShowState(computeStateFlags("_out_toIn_"), stateID, doAnimation, menuAnimationListener, false);
    }
}

void UIView::show(bool doAnimation) {
    show(doAnimation, nullptr, nullptr);
}

// Extendable.
void UIView::onShowRequested_pre() {
    //void
}

void UIView::hide(bool doAnimation, IAnimationListener* menuAnimationListener) {
    if (!isActive()) {
        throw LogicException(LOC);
    }

    gotoShowState(computeStateFlags("_out_"), doAnimation, menuAnimationListener, false);

    if (!isAnyAppLoopCalled) {
        throw LogicException(LOC);
    }
}

void UIView::hide(bool doAnimation) {
    hide(doAnimation, nullptr);
}

void UIView::onSelectedStateChanged() {
    super::onSelectedStateChanged();

    if (!isActive()) {
        //void
    } else {
        //void
    }
}

UIView::~UIView() {
    //void
}
