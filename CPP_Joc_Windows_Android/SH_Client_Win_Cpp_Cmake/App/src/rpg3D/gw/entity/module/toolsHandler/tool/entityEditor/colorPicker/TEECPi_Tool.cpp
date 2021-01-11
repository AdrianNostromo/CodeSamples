#include "TEECPi_Tool.h"
#include <rpg3D/gw/entity/module/voxelizedEditor/IVoxelizedEditorM_Edit.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/math/collision/Ray.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <base/audio/sound/ISound.h>
#include "config/ToolConfigEEColorPicker.h"

using namespace rpg3D;

const int TEECPi_Tool::TouchMode_TOOL = getNew_touchModeIncrementor();

TEECPi_Tool::TEECPi_Tool(ToolConfigEEColorPicker* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData)
{
	//void
}

int TEECPi_Tool::computeTouchModeUnderPoint(float screenX, float screenY) {
	Ray& ray = entityEditor->getCinematicController()->peekCamera()->getPickRay(screenX, screenY, true);
	std::shared_ptr<IEditableVoxelisedM_Edit::TouchVoxelTarget> touchedVoxelResult = entityEditor->getEntityToEdit_editableEntity()->getClosestVoxelForRay(ray, false);
	if (touchedVoxelResult != nullptr) {
		return TouchMode_TOOL;
	}

	return super::computeTouchModeUnderPoint(screenX, screenY);
}

void TEECPi_Tool::onIsOperatingChanged(bool isOperating) {
	super::onIsOperatingChanged(isOperating);

	if (!isOperating) {
		// Do clearing on the deactivation because doing it on activation may cause problems with certain activation logics.
		isInitialTouchedVoxel = false;
	}
}

void TEECPi_Tool::tickActiveTouchMode(
	int touchMode, float deltaS,
	Vector2& screenPos,
	Vector2& currentDragDeltaSensitised)
{
	super::tickActiveTouchMode(
		touchMode, deltaS,
		screenPos,
		currentDragDeltaSensitised
	);

	if (touchMode == TouchMode_TOOL) {
		bool isPlayActivationSound = false;
		if (!isInitialTouchedVoxel) {
			isPlayActivationSound = true;
		}

		Ray& ray = entityEditor->getCinematicController()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
		std::shared_ptr<IEditableVoxelisedM_Edit::TouchVoxelTarget> touchedVoxelResult = entityEditor->getEntityToEdit_editableEntity()->getClosestVoxelForRay(ray, false);

		// Make sure the paint focus is on the entity voxels surface and not besides the entity.
		if (touchedVoxelResult != nullptr) {
			int registeredVertexColorIndex = entityEditor->getEntityToEdit_editableEntity()->getVoxelMaterialIndex(touchedVoxelResult->gPos.x, touchedVoxelResult->gPos.y, touchedVoxelResult->gPos.z);

			if (!isInitialTouchedVoxel || registeredVertexColorIndex != lastTouched_registeredVertexColorIndex) {
				// Select the color used.
				lastTouched_registeredVertexColorIndex = registeredVertexColorIndex;
				isInitialTouchedVoxel = true;

				int matIndex;
				entityEditor->getEntityToEdit_editableEntity()->getEditorMaterialWithRegisteredVertexColorIndex(registeredVertexColorIndex, &matIndex);

				if (matIndex != entityEditor->getEntityToEdit_editableEntity()->peekActiveDynamicMaterialIndex()) {
					entityEditor->getEntityToEdit_editableEntity()->setActiveDynamicMaterialIndex(matIndex);

					isPlayActivationSound = true;
				}
			}
		}

		if (isPlayActivationSound) {
			if (activationSound != nullptr) {
				activationSound->play(config->activationSoundConfig_volume);
			}
		}
	}
}

TEECPi_Tool::~TEECPi_Tool() {
	//void
}
