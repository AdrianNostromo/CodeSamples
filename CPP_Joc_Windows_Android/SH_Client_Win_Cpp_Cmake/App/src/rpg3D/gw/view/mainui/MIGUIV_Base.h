#pragma once

#include <base/view/UIView.h>
#include <functional>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include <base/input/util/IBlockingInputLayer.h>
#include "IMainInGameUIView.h"

namespace base {
    class IGameWorld;
};

namespace rpg3D {
class MIGUIV_Base : public UIView, virtual public IMainInGameUIView {priv typedef UIView super;pub dCtor(MIGUIV_Base);
    pub class IPanelHandler {
        pub virtual void onRequestShowMainSidebarView() = 0;
        pub virtual void onRequestToggleCameraMode() = 0;

        pub virtual ~IPanelHandler() = default;
    };
    pub class CbPanelHandler : virtual public IPanelHandler {
        pub std::function<void()> cb_onRequestShowMainSidebarView = nullptr;
        pub std::function<void()> cb_onRequestToggleCameraMode = nullptr;

        pub void onRequestShowMainSidebarView() override {
            if(cb_onRequestShowMainSidebarView) {
                cb_onRequestShowMainSidebarView();
            }
        }
        pub void onRequestToggleCameraMode() override {
            if(cb_onRequestToggleCameraMode) {
                cb_onRequestToggleCameraMode();
            }
        }

        pub ~CbPanelHandler() override = default;
    };

    prot base::IGameWorld* gw;

    prot IPanelHandler* handler = nullptr;

    priv bool isFirstPersonStarshipEditorActive = false;
    priv bool isShipyardModeStarshipEditorActive = false;

    priv int cameraModeBitGroup = GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation;

    priv base::IBlockingInputLayer* blockingStreamLayer = nullptr;

    priv bool isCrosshair = false;
    priv bool isEditorModeActive = false;

    pub explicit MIGUIV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw);

    pub void setHandler(IPanelHandler* handler);

    pub bool getIsCrosshair() override;
    pub void setIsCrosshair(bool isCrosshair) override;
	
    pub bool getIsEditorModeActive() override;
    pub void setIsEditorModeActive(bool isEditorModeActive) override;

    pub base::IBlockingInputLayer* getBlockingStreamLayer();
    pub void setBlockingStreamLayer(base::IBlockingInputLayer* blockingStreamLayer);

    pub template <typename T>
    T getHandlerAs();

    pub virtual void onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS);

    pub void setIsFirstPersonStarshipEditorActive(bool isFirstPersonStarshipEditorActive);
    pub void setIsShipyardModeStarshipEditorActive(bool isShipyardModeStarshipEditorActive);

    pub void setCameraMode(int cameraModeBitGroup);

    pub std::string computeStateFlags(std::string baseState) override;

    pub ~MIGUIV_Base() override;
};

template <typename T>
T MIGUIV_Base::getHandlerAs() {
    if(!handler) {
        return nullptr;
    }

    T h = dynamic_cast<T>(handler);
    if(!h) {
        throw LogicException(LOC);
    }

    return h;
}

};
