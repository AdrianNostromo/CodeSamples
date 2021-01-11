#include "SVBase.h"
#include <base/exceptions/LogicException.h>
#include <sh/app/IAppSquareHeads.h>

using namespace sh;

SVBase::SVBase(
    IAppSquareHeads* app,
    ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList)
    : super(app, viewItemConfigs, viewEaseDurationsSList, nullptr/*handlerAutoDisposal*/),
    app(app)
{
    //void
}

IAppSquareHeads* SVBase::getApp() {
    return app;
}

int SVBase::getActiveStatusType() {
    return activeStatusType;
}

bool SVBase::getIsConsoleActive() {
    return isConsoleActive;
}

void SVBase::toggleConsole() {
    bool newIsConsoleActive;
    if(isConsoleActive) {
        newIsConsoleActive = false;
    }else {
        newIsConsoleActive = true;
    }
    setIsConsoleActive(newIsConsoleActive);

    updateShowStateIfChanged(true, nullptr);
}

void SVBase::setIsConsoleActive(bool isConsoleActive) {
    if(this->isConsoleActive == isConsoleActive) {
        return ;
    }

    this->isConsoleActive = isConsoleActive;

    if(!this->isConsoleActive) {
        removeViewActiveElement();
    }else {
        addViewActiveElement();
    }

    updateShowStateIfChanged(true, nullptr);
}

void SVBase::activateStatus(int statusType, float durationS) {
    if(this->activeStatusType == statusType) {
        return ;
    }

    int oldActiveStatusType = this->activeStatusType;

    this->activeStatusType = statusType;
    this->activeStatusTargetDurationS = durationS;
    this->activeStatusLifeS = 0.0f;

    if(oldActiveStatusType != StatusType::None && this->activeStatusType == StatusType::None) {
        removeViewActiveElement();
    }else if(oldActiveStatusType == StatusType::None && this->activeStatusType != StatusType::None) {
        addViewActiveElement();
    }

    if((this->activeStatusType == StatusType::ConnectToServer_error || this->activeStatusType == StatusType::CheckInternet_error)
        && !isFlaserPlayed)
    {
        isFlaserPlayed = true;

        startFlash();
    }else if(this->activeStatusType == StatusType::None || this->activeStatusType == StatusType::General_ok) {
        // Reset the flasher so it appear on the next error again.
        isFlaserPlayed = false;
    }

    updateShowStateIfChanged(true, nullptr);
}

void SVBase::setIsInternet(bool isInternet) {
    this->flagStatus_isInternet = isInternet;

    updateShowStateIfChanged(true, nullptr);
}

void SVBase::setIsConnectedToServer(bool isConnectedToServer) {
    this->flagStatus_isConnectedToServer = isConnectedToServer;

    updateShowStateIfChanged(true, nullptr);
}

void SVBase::visualLoop(float deltaS, int deltaMS) {
    super::visualLoop(deltaS, deltaMS);

    if(activeStatusType != StatusType::None && activeStatusTargetDurationS >= 0.0f) {
        activeStatusLifeS += deltaS;

        if(activeStatusLifeS >= activeStatusTargetDurationS) {
            activateStatus(StatusType::None, -1.0f);
        }
    }
}

std::string SVBase::computeStateFlags(std::string baseState) {
    std::string state = super::computeStateFlags(baseState);

    //state += "_SOME-FLAG_";

    return state;
}

bool SVBase::getIsViewActive() {
    if(activeElementsCount > 0) {
        return true;
    }

    return false;
}

float SVBase::getInactiveViewIdleTimerS() {
    return inactiveViewIdleTimerS;
}

void SVBase::setInactiveViewIdleTimerS(float inactiveViewIdleTimerS) {
    this->inactiveViewIdleTimerS = inactiveViewIdleTimerS;
}

void SVBase::addViewActiveElement() {
    activeElementsCount++;
    inactiveViewIdleTimerS = 0.0f;
}

void SVBase::removeViewActiveElement() {
    if(activeElementsCount <= 0) {
        throw LogicException(LOC);
    }

    activeElementsCount--;
    inactiveViewIdleTimerS = 0.0f;
}

SVBase::~SVBase() {
    //void
}
