#include "GPUIMainUIView.h"
#include <rpg3D/gw/view/mainui/MainInGameUIView.h>
#include <rpg3D/gw/userEntitiesController/util/EntityControlType.h>
#include <rpg3D/gw/sim/GamePartSim.h>
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <rpg3D/gw/content/IGPContent.h>
#include <worldGame3D/gw/interaction/IGPInteraction.h>
#include <rpg3D/gw/userEntitiesController/IGPUserEntitiesController.h>
#include <worldGame3D/gw/main/IGamePartMain.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

GPUIMainUIView::GPUIMainUIView(base::IGameWorld* gw)
	: super(gw)
{
	gw->addEventListener(
		IGPContent::GWEvent_onSelectedPlayerEntityChanged->type, 
		base::IGPSim::GWEvent_onCameraModeChanged->type,
		std::bind(&GPUIMainUIView::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);

	gw->addEventListener(
		base::IGameWorld::GWEvent_gameLoop_localInput->type,
		std::bind(&GPUIMainUIView::onGWEvent_gameLoop_localInput, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void GPUIMainUIView::setNewMainUIViewUninitialised(MainInGameUIView* view) {
	if (mainUiView != nullptr) {
		mainUiView->setHandler(nullptr);

		gw->getViewComponentsHandler()->removeComponent(mainUiView, true/*doDispose*/);

		mainUiView = nullptr;
	}

	if (view != nullptr) {
		mainUiView = view;

		mainUiView->setHandler(&panelHandler);

		mainUiView->setBlockingStreamLayer(gw->getInteractionAs<base::IGPInteraction*>()->getInputAs<IManagedDeviceInput*>()->getBlockingInputLayer());
		gw->getViewComponentsHandler()->addComponent(mainUiView, true/*doCreate*/);
		gw->getGPMain()->getLayerMainUserInterface()->addChild(mainUiView->getVisualsHolder());

		mainUiView->setCameraMode(gw->getGPSim()->getCameraMode());

		if (mainUIListenerDPad != nullptr) {
			mainUiView->setListenerDPad(this->mainUIListenerDPad);
		}
		if (mainUIListenerToolExtraActivation != nullptr) {
			mainUiView->setListenerToolExtraActivation(this->mainUIListenerToolExtraActivation);
		}

		if (gw->getContent()->getSelectedPlayerEntity(false/*mustExist*/) != nullptr) {
			mainUiView->setSelectedFocusEntity(gw->getContent()->getSelectedPlayerEntity(false/*mustExist*/));
		}

		if (showViewCounter >= 1) {
			mainUiView->setIsCrosshair(gw->getSim()->getCameraMode() == GameCameraModeBitGroups::followerFirstPersonTargetRotation ? true : false);
			mainUiView->setIsEditorModeActive(mainUIMinimalModeCounter > 0 ? true : false);
			mainUiView->show(false, nullptr, nullptr);
		}
	}
}

IMainInGameUIView* GPUIMainUIView::getMainUIView() {
	return mainUiView;
}

void GPUIMainUIView::onGWEvent_gameLoop_localInput(IEventListener& eventListener, base::GWEvent& event) {
	if(mainUiView != nullptr) {
		mainUiView->onGameLoop_localInput(event.deltaS, event.deltaMS, event.gameTimeS);
	}
}

void GPUIMainUIView::createBMain() {
	super::createBMain();

	panelHandler.cb_onRequestShowMainSidebarView = std::bind(&GPUIMainUIView::ui_onRequestShowMainSidebarView, this);
	panelHandler.cb_onRequestToggleCameraMode = std::bind(&GPUIMainUIView::ui_onRequestToggleCameraMode, this);
}

void GPUIMainUIView::onEvent(IEventListener& eventListener, base::GWEvent& event) {
	if(event.type == IGPContent::GWEvent_onSelectedPlayerEntityChanged->type) {
		if(mainUiView != nullptr) {
			mainUiView->setSelectedFocusEntity(gw->getContent()->getSelectedPlayerEntity(false/*mustExist*/));
		}
	} else if (event.type == base::IGPSim::GWEvent_onCameraModeChanged->type) {
		if (mainUiView != nullptr) {
			mainUiView->setCameraMode(gw->getSim()->getCameraMode());

			if (mainUiView->isActive()) {
				mainUiView->setIsCrosshair(gw->getSim()->getCameraMode() == GameCameraModeBitGroups::followerFirstPersonTargetRotation ? true : false);
			}
		}
	}else {
		throw LogicException(LOC);
	}
}

void GPUIMainUIView::setMainUiDPadListener(IMainInGameUIView::IListenerDPad* listenerDPad) {
	this->mainUIListenerDPad = listenerDPad;
	if(mainUiView != nullptr) {
		mainUiView->setListenerDPad(this->mainUIListenerDPad);
	}
}

void GPUIMainUIView::setMainUiToolExtraActivationListener(IMainInGameUIView::IListenerToolExtraActivation* listenerToolExtraActivation) {
	this->mainUIListenerToolExtraActivation = listenerToolExtraActivation;
	if(mainUiView != nullptr) {
		mainUiView->setListenerToolExtraActivation(this->mainUIListenerToolExtraActivation);
	}
}

void GPUIMainUIView::ui_onRequestShowMainSidebarView() {
	gw->getHandler()->onGWEvent(base::IGameWorld::GWE_RequestShowMainSidebarView, nullptr);
}

void GPUIMainUIView::ui_onRequestToggleCameraMode() {
	int newCameraMode;
	int newEntityControlType;

	if(gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->getEntityControlType() == EntityControlType::DPadUnitAxesStrafe_CursorRotationBothAxes) {
		newCameraMode = GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation;

		newEntityControlType = EntityControlType::DPadCameraAxesDirectionMove_LookAtCursorOrAutoRotToMoveDirection;
	}else if(gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->getEntityControlType() == EntityControlType::DPadCameraAxesDirectionMove_LookAtCursorOrAutoRotToMoveDirection) {
		newCameraMode = GameCameraModeBitGroups::followerFirstPersonTargetRotation;

		newEntityControlType = EntityControlType::DPadUnitAxesStrafe_CursorRotationBothAxes;
	} else if (gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->getEntityControlType() == EntityControlType::GridTurnBased) {
		if (gw->getGPSim()->getCameraMode() == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
			newCameraMode = GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation;
		} else {
			newCameraMode = GameCameraModeBitGroups::followerFirstPersonTargetRotation;
		}

		newEntityControlType = EntityControlType::GridTurnBased;
	}else {
		throw LogicException(LOC);
	}

	gw->getGPSim()->setCameraMode(newCameraMode, true);
	gw->getComponentAs<rpg3D::IGPUserEntitiesController*>(true/*mustExist*/)->setEntityControlType(newEntityControlType);
}

void GPUIMainUIView::showViewIncremental() {
	showViewCounter++;

	if (showViewCounter > 1) {
		// The current system requires hide to be called and show after.
		throw LogicException(LOC);
	}
	if(showViewCounter == 1 && mainUiView != nullptr) {
		mainUiView->setIsCrosshair(gw->getSim()->getCameraMode() == GameCameraModeBitGroups::followerFirstPersonTargetRotation ? true : false);
		mainUiView->setIsEditorModeActive(mainUIMinimalModeCounter > 0 ? true : false);
		mainUiView->show(false, nullptr, nullptr);
	}
}

void GPUIMainUIView::hideViewIncremental() {
	showViewCounter--;

	if(showViewCounter == 0) {
		mainUiView->hide(false, nullptr);
	}
}

void GPUIMainUIView::activateMainUIMinimalModeIncremental() {
	mainUIMinimalModeCounter++;

	if (mainUIMinimalModeCounter > 1) {
		// The current system requires hide to be called and show after.
		throw LogicException(LOC);
	}
	if (mainUIMinimalModeCounter == 1 && mainUiView != nullptr && mainUiView->isActive()) {
		mainUiView->setIsEditorModeActive(true);
	}
}

void GPUIMainUIView::deactivateMainUIMinimalModeIncremental() {
	mainUIMinimalModeCounter--;

	if (mainUIMinimalModeCounter == 0 && mainUiView != nullptr && mainUiView->isActive()) {
		mainUiView->setIsEditorModeActive(false);
	}
}

void GPUIMainUIView::disposeMain() {
	if(mainUiView != nullptr) {
		gw->getViewComponentsHandler()->removeComponent(mainUiView, true/*doDispose*/);

		mainUiView = nullptr;
	}

	super::disposeMain();
}

GPUIMainUIView::~GPUIMainUIView() {
	//void
}
