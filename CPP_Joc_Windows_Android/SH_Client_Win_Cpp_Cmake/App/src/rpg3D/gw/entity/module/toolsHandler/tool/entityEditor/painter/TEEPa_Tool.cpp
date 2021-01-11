#include "TEEPa_Tool.h"
#include <rpg3D/gw/entity/module/voxelizedEditor/IVoxelizedEditorM_Edit.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/math/collision/Ray.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>

using namespace rpg3D;

const int TEEPa_Tool::TouchMode_TOOL = getNew_touchModeIncrementor();

TEEPa_Tool::TEEPa_Tool(ToolConfigEEPainter* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData)
{
	//void
}

int TEEPa_Tool::computeTouchModeUnderPoint(float screenX, float screenY) {
	Ray& ray = entityEditor->getCinematicController()->peekCamera()->getPickRay(screenX, screenY, true);
	std::shared_ptr<IEditableVoxelisedM_Edit::TouchVoxelTarget> touchedVoxelResult = entityEditor->getEntityToEdit_editableEntity()->getClosestVoxelForRay(ray, false);
	if (touchedVoxelResult != nullptr) {
		return TouchMode_TOOL;
	}

	return super::computeTouchModeUnderPoint(screenX, screenY);
}

void TEEPa_Tool::onIsOperatingChanged(bool isOperating) {
	super::onIsOperatingChanged(isOperating);

	if (!isOperating) {
		// Do clearing on the deactivation because doing it on activation may cause problems with certain activation logics.
		lastDrawnVoxel = nullptr;
	}
}

void TEEPa_Tool::tickActiveTouchMode(
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
		Ray& ray = entityEditor->getCinematicController()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
		std::shared_ptr<IEditableVoxelisedM_Edit::TouchVoxelTarget> touchedVoxelResult = entityEditor->getEntityToEdit_editableEntity()->getClosestVoxelForRay(ray, false);

		// Make sure the paint focus is on the entity voxels surface and not besides the entity.
		if (touchedVoxelResult != nullptr && entityEditor->getEntityToEdit_editableEntity()->peekActiveDynamicMaterialIndex() >= 0) {
			// Get the touched voxel. If different from the last one, process it.
			if (lastDrawnVoxel == nullptr || !lastDrawnVoxel->equals(touchedVoxelResult->gPos)) {
				if (lastDrawnVoxel == nullptr) {
					lastDrawnVoxel = std::make_shared<Vector3Int>(touchedVoxelResult->gPos);
				} else {
					lastDrawnVoxel->set(touchedVoxelResult->gPos);
				}

				entityEditor->getEntityToEdit_editableEntity()->setVoxelMaterialIndex(touchedVoxelResult->gPos.x, touchedVoxelResult->gPos.y, touchedVoxelResult->gPos.z, entityEditor->getEntityToEdit_editableEntity()->peekActiveDynamicMaterialIndex(), true);
			}
		}
	}
}

TEEPa_Tool::~TEEPa_Tool() {
	//void
}
