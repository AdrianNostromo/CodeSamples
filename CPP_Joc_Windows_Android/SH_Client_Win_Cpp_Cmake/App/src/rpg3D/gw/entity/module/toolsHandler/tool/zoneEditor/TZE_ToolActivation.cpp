#include "TZE_ToolActivation.h"
#include <rpg3D/gw/entity/module/zoneEditor/IZoneEditorModule.h>
#include <rpg3D/gw/entity/template/zoneEditor/ZoneEditorTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/audio/sound/ISound.h>
#include "config/ToolConfigZoneEditor.h"
#include "util/ToolActivationInfo_Base.h"

using namespace rpg3D;

int TZE_ToolActivation::getNew_touchModeIncrementor() {
	static int touchModeIncrementor = 0;
	int ret = touchModeIncrementor;
	touchModeIncrementor++;

	return ret;
}

const int TZE_ToolActivation::TouchMode_IDDLE = getNew_touchModeIncrementor();

TZE_ToolActivation::TZE_ToolActivation(ToolConfigZoneEditor* config, std::shared_ptr<ExtraData> extraData,
	bool usesAutoToolModeActivationSound)
	: super(config, extraData),
	usesAutoToolModeActivationSound(usesAutoToolModeActivationSound)
{
	//void
}

void TZE_ToolActivation::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_simulationC(deltaS, deltaMS, gameTimeS);

	if (isDragActive) {
		if ((isTouchDownPulse || !isTouchDragStopRequested) && getIsOperatingAny()) {
			// This is also received on the initial pulse tick even if isTouchDragStopRequested already exists.

			if (isTouchDownPulse && touchMode == TouchMode_IDDLE) {
				int newTouchMode = computeTouchModeUnderPoint(currentDrag_touchData.get());
				if (newTouchMode == TouchMode_IDDLE) {
					//void
				}
				if (newTouchMode != TouchMode_IDDLE && usesAutoToolModeActivationSound) {
					if (activationSound != nullptr) {
						activationSound->play(config->activationSoundConfig_volume);
					}
				}
				
				setTouchMode(newTouchMode);
			}

			if (touchMode != TouchMode_IDDLE) {
				//asdA35;// rename to activatePulse.
				//asdA35;// implement a optionally_used cooldown system.
				tickActiveTouchMode(touchMode, deltaS,
					currentDrag_touchData.get()
				);
			}

			currentDrag_touchData->clearCumulators();

			isTouchDownPulse = false;
		}

		if (isTouchDragStopRequested || !getIsOperatingAny()) {
			isDragActive = false;

			if (touchMode != TouchMode_IDDLE) {
				setTouchMode(TouchMode_IDDLE);
			}
		}
	}
}

void TZE_ToolActivation::tickActiveTouchMode(
	int touchMode, float deltaS,
	ToolActivationInfo_Base* touchData)
{
	//void
}

int TZE_ToolActivation::getTouchMode() {
	return touchMode;
}

void TZE_ToolActivation::setTouchMode(int touchMode) {
	if (this->touchMode == touchMode) {
		return;
	}

	int oldTouchMode = this->touchMode;
	this->touchMode = touchMode;
	onTouchModeChange(this->touchMode, oldTouchMode);
}

void TZE_ToolActivation::stopCurrentTouchMode() {
	if (touchMode != TouchMode_IDDLE) {
		setTouchMode(TouchMode_IDDLE);
	}
}

void TZE_ToolActivation::onTouchModeChange(int touchMode, int oldTouchMode) {
	//void
}

// Fully extendable.
int TZE_ToolActivation::computeTouchModeUnderPoint(ToolActivationInfo_Base* touchData) {
	return TouchMode_IDDLE;
}

void TZE_ToolActivation::toolFilterStart(sp<ToolActivationInfo_Base> touchData) {
	this->currentDrag_touchData = touchData;
	this->currentDrag_touchData->clearCumulators();

	isTouchDownPulse = true;
	isDragActive = true;
	isTouchDragStopRequested = false;
}

void TZE_ToolActivation::toolFilterUpdate(sp<ToolActivationInfo_Base> touchData) {
	this->currentDrag_touchData = touchData;
}

void TZE_ToolActivation::toolFilterEnd(sp<ToolActivationInfo_Base> touchData) {
	setIsOperating(false, touchData->cursorIndex);

	isTouchDragStopRequested = true;
}

void TZE_ToolActivation::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		entityEditor = nullptr;
	} else {
		entityEditor = static_cast<IZoneEditorM_Edit*>(entity->getComponentAs<rpg3D::zoneEditorModule::IZoneEditorModule*>(true/*mustExist*/));
	}
}

void TZE_ToolActivation::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		//void
	} else {
		//void
	}
}

TZE_ToolActivation::~TZE_ToolActivation() {
	//void
}
