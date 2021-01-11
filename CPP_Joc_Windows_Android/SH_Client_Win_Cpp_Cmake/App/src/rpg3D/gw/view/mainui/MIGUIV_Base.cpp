#include "MIGUIV_Base.h"
#include <base/exceptions/LogicException.h>

using namespace rpg3D;

MIGUIV_Base::MIGUIV_Base(IApp* app, ArrayList<MenuItemConfig*>* viewItemConfigs, ArrayList<StateChangeDurations*>* viewEaseDurationsSList, base::IGameWorld* gw)
    : super(app, viewItemConfigs, viewEaseDurationsSList, nullptr/*handlerAutoDisposal*/),
    gw(gw)
{
    //void
}

void MIGUIV_Base::setHandler(IPanelHandler* handler) {
	this->handler = handler;
}

bool MIGUIV_Base::getIsCrosshair() {
	return isCrosshair;
}

void MIGUIV_Base::setIsCrosshair(bool isCrosshair) {
	if (this->isCrosshair == isCrosshair) {
		return;
	}

	this->isCrosshair = isCrosshair;

	updateShowStateIfChanged(true, nullptr);
}

bool MIGUIV_Base::getIsEditorModeActive() {
	return isEditorModeActive;
}

void MIGUIV_Base::setIsEditorModeActive(bool isEditorModeActive) {
	if (this->isEditorModeActive == isEditorModeActive) {
		return;
	}

	this->isEditorModeActive = isEditorModeActive;

	updateShowStateIfChanged(true, nullptr);
}

base::IBlockingInputLayer* MIGUIV_Base::getBlockingStreamLayer() {
    return blockingStreamLayer;
}

void MIGUIV_Base::setBlockingStreamLayer(base::IBlockingInputLayer* blockingStreamLayer) {
    this->blockingStreamLayer = blockingStreamLayer;
}

void MIGUIV_Base::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
    //void
}

void MIGUIV_Base::setIsFirstPersonStarshipEditorActive(bool isFirstPersonStarshipEditorActive) {
    this->isFirstPersonStarshipEditorActive = isFirstPersonStarshipEditorActive;

    updateShowStateIfChanged(true, nullptr);
}

void MIGUIV_Base::setIsShipyardModeStarshipEditorActive(bool isShipyardModeStarshipEditorActive) {
    this->isShipyardModeStarshipEditorActive = isShipyardModeStarshipEditorActive;

    updateShowStateIfChanged(true, nullptr);
}

void MIGUIV_Base::setCameraMode(int cameraModeBitGroup) {
    this->cameraModeBitGroup = cameraModeBitGroup;

    updateShowStateIfChanged(true, nullptr);
}

std::string MIGUIV_Base::computeStateFlags(std::string baseState) {
    std::string state = super::computeStateFlags(baseState);

    if(cameraModeBitGroup == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
        state += "firstPersonCameraMode_";
    }else if(cameraModeBitGroup == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
        state += "thirdPersonCameraMode_";
    }else {
        throw LogicException(LOC);
    }

    if(isFirstPersonStarshipEditorActive || isShipyardModeStarshipEditorActive) {
        state += "isStarshipEditorActive_";
    }

	if (getIsCrosshair()) {
		state += "_isCrosshair_";
	}
	
	if (getIsEditorModeActive()) {
		state += "_editorMode_";
		state += "_minimalParts_";
	} else {
		state += "_fullMode_";
		state += "_minimalParts_fullParts_";
	}

    return state;
}

MIGUIV_Base::~MIGUIV_Base() {
    //void
}
