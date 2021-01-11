#pragma once

#include <base/gh.h>
#include <base/view/UIView.h>
#include "IStatusView.h"

class IEventListener;
namespace base {
    class LoopEvent;
};
namespace sh {
	class IAppSquareHeads;
};

namespace sh {
class SVBase : public UIView, virtual public IStatusView {priv typedef UIView super;pub dCtor(SVBase);
    priv IAppSquareHeads* app;

    priv int activeStatusType = StatusType::None;
    priv float activeStatusTargetDurationS = -1.0f;
    // Incremented only if there is a expiration.
    priv float activeStatusLifeS = 0.0f;

    priv bool isConsoleActive = false;

    //asdA9;// set and use these.
    priv bool flagStatus_isInternet = false;
    priv bool flagStatus_isConnectedToServer = false;

    // This is the number of active visuals in this view (eg: indicator, console, flasher).
    // When this is 0 for 0.5 seconds, this view is hide(false) and disposed;
    priv int activeElementsCount = 0;
    priv float inactiveViewIdleTimerS = 0.0f;

    // This is reset (to false) on some statuses (None, General_ok).
    priv bool isFlaserPlayed = false;

    pub explicit SVBase(
        IAppSquareHeads* app,
        ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList);

    pub void activateStatus(int statusType, float durationS) override;

    pub void setIsInternet(bool isInternet) override;
    pub void setIsConnectedToServer(bool isConnectedToServer) override;

    pub void visualLoop(float deltaS, int deltaMS) override;

    pub std::string computeStateFlags(std::string baseState) override;

    pub bool getIsViewActive();
    pub float getInactiveViewIdleTimerS();
    pub void setInactiveViewIdleTimerS(float inactiveViewIdleTimerS);

    prot IAppSquareHeads* getApp();

    prot int getActiveStatusType();

    prot bool getIsConsoleActive();
    prot void toggleConsole();
    prot void setIsConsoleActive(bool isConsoleActive);

    prot virtual void startFlash() = 0;

    prot void addViewActiveElement();
    prot void removeViewActiveElement();

    pub ~SVBase() override;
};
};
