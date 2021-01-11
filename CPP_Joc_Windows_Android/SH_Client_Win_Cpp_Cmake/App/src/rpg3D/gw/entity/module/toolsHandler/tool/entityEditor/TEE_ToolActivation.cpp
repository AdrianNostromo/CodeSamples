#include "TEE_ToolActivation.h"
#include <rpg3D/gw/entity/module/voxelizedEditor/IVoxelizedEditorModule.h>
#include <rpg3D/gw/entity/template/voxelizedEditor/VoxelizedEditorTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/audio/sound/ISound.h>
#include "config/ToolConfigEntityEditor.h"

using namespace rpg3D;

int TEE_ToolActivation::getNew_touchModeIncrementor() {
	static int touchModeIncrementor = 0;
	int ret = touchModeIncrementor;
	touchModeIncrementor++;

	return ret;
}

const int TEE_ToolActivation::TouchMode_IDDLE = getNew_touchModeIncrementor();

TEE_ToolActivation::TEE_ToolActivation(ToolConfigEntityEditor* config, std::shared_ptr<ExtraData> extraData,
	bool usesAutoToolModeActivationSound)
	: super(config, extraData),
	usesAutoToolModeActivationSound(usesAutoToolModeActivationSound)
{
	//void
}

void TEE_ToolActivation::onGameLoop_simulationC(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_simulationC(deltaS, deltaMS, gameTimeS);

	if (isDragActive) {
		if (isTouchDownPulse || !isTouchDragStopRequested) {
			// This is also received on the initial pulse tick even if isTouchDragStopRequested already exists.

			if (isTouchDownPulse && touchMode == TouchMode_IDDLE) {
				int newTouchMode = computeTouchModeUnderPoint(currentDrag_screenPos.x, currentDrag_screenPos.y);
				if (newTouchMode == TouchMode_IDDLE) {
					if (entityEditor->getEntityToEdit_editableEntity()->getIsEditorStructureEnabled()) {
						// Rotation should occur in this case.

						throw LogicException(LOC);
					}
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
					currentDrag_screenPos,
					currentDrag_deltaSensitised
				);
			}

			currentDrag_deltaSensitised.idt();

			isTouchDownPulse = false;
		}

		if (isTouchDragStopRequested) {
			isDragActive = false;

			if (touchMode != TouchMode_IDDLE) {
				setTouchMode(TouchMode_IDDLE);
			}
		}
	}
}

void TEE_ToolActivation::tickActiveTouchMode(
	int touchMode, float deltaS,
	Vector2& screenPos,
	Vector2& currentDragDeltaSensitised)
{
	//void
}

int TEE_ToolActivation::getTouchMode() {
	return touchMode;
}

void TEE_ToolActivation::setTouchMode(int touchMode) {
	if (this->touchMode == touchMode) {
		return;
	}

	int oldTouchMode = this->touchMode;
	this->touchMode = touchMode;
	onTouchModeChange(this->touchMode, oldTouchMode);
}

void TEE_ToolActivation::stopCurrentTouchMode() {
	if (touchMode != TouchMode_IDDLE) {
		setTouchMode(TouchMode_IDDLE);
	}
}

void TEE_ToolActivation::onTouchModeChange(int touchMode, int oldTouchMode) {
	//void
}

// Fully extendable.
int TEE_ToolActivation::computeTouchModeUnderPoint(float screenX, float screenY) {
	return TouchMode_IDDLE;
}

void TEE_ToolActivation::toolFilterStart(Vector2& screenPos) {
	currentDrag_screenPos.set(screenPos);
	currentDrag_deltaSensitised.idt();

	isTouchDownPulse = true;
	isDragActive = true;
	isTouchDragStopRequested = false;
}

void TEE_ToolActivation::toolFilterUpdate(Vector2& screenPos, Vector2& sensitisedScreenCursorMoveDelta) {
	currentDrag_screenPos.set(screenPos);
	currentDrag_deltaSensitised.add(sensitisedScreenCursorMoveDelta);
}

void TEE_ToolActivation::toolFilterEnd(int cursorIndex) {
	setIsOperating(false, cursorIndex);

	isTouchDragStopRequested = true;
}

void TEE_ToolActivation::onEntityChanged() {
	super::onEntityChanged();

	if (entity == nullptr) {
		entityEditor = nullptr;
	} else {
		entityEditor = static_cast<IVoxelizedEditorM_Edit*>(entity->getComponentAs<rpg3D::entityEditorModule::IVoxelizedEditorModule*>(true/*mustExist*/));
	}
}

void TEE_ToolActivation::onSelectedAndGWCapabilitiesEnabledStateChanged(bool isSelectedAndGWCapabilitiesExist, worldGame3D::IGameWorldGameParts* gwGamePartsOptional) {
	super::onSelectedAndGWCapabilitiesEnabledStateChanged(isSelectedAndGWCapabilitiesExist, gwGamePartsOptional);

	if (!isSelectedAndGWCapabilitiesExist) {
		//void
	} else {
		//void
	}
}

TEE_ToolActivation::~TEE_ToolActivation() {
	//void
}
