#include "TEE_Rotate.h"
#include <rpg3D/gw/entity/template/voxelizedEditor/VoxelizedEditorTemplate.h>
#include <rpg3D/gw/entity/module/voxelizedEditor/IVoxelizedEditorModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <graphics/visual3d/drawable/IContainer3D.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <worldGame3D/gw/entity/module/visual3D/IVisual3DModule.h>

using namespace rpg3D;

const float TEE_Rotate::dragSpeedMultiplier = 6.0f;
const int TEE_Rotate::TouchMode_ROTATE = getNew_touchModeIncrementor();

TEE_Rotate::TEE_Rotate(ToolConfigEntityEditor* config, std::shared_ptr<ExtraData> extraData,
	bool usesAutoToolModeActivationSound)
	: super(config, extraData,
		usesAutoToolModeActivationSound)
{
	//void
}

int TEE_Rotate::computeTouchModeUnderPoint(float screenX, float screenY) {
	if (entityEditor->getEntityToEdit_editableEntity()->getIsEditorStructureEnabled()) {
		return TouchMode_ROTATE;
	}

	return super::computeTouchModeUnderPoint(screenX, screenY);
}

void TEE_Rotate::onTouchModeChange(int touchMode, int oldTouchMode) {
	super::onTouchModeChange(touchMode, oldTouchMode);

	if (getTouchMode() == TouchMode_ROTATE) {
		isFirstRotationTickSkiped = false;
	}
}

void TEE_Rotate::tickActiveTouchMode(
	int touchMode, float deltaS,
	Vector2& screenPos,
	Vector2& currentDragDeltaSensitised)
{
	super::tickActiveTouchMode(
		touchMode, deltaS,
		screenPos,
		currentDragDeltaSensitised
	);

	if (touchMode == TouchMode_ROTATE) {
		if (!isFirstRotationTickSkiped) {
			isFirstRotationTickSkiped = true;

			return;
		}

		if (currentDragDeltaSensitised.x != 0.0f || currentDragDeltaSensitised.y != 0.0f) {
			Vector3 camDir{ entityEditor->getCinematicController()->peekCamera()->getDirection() };
			Vector3 upDir{ Vector3::Z };
			Vector3 rightDir{ camDir };
			rightDir.crs(upDir);
			Vector3 angledZDir{ camDir };
			angledZDir.crs(rightDir);

			Quaternion rot1{ angledZDir, -currentDragDeltaSensitised.x * dragSpeedMultiplier };
			// Invert the drag y to convert from top_left to bottom_left coordonates.
			Quaternion rot2{ rightDir, -currentDragDeltaSensitised.y * dragSpeedMultiplier };

			Quaternion rot{};

			// Rotate with the inverse of the root drawable to have the right reference frame.
			Quaternion qRoodDrawableRot{ *entityEditor->getEntityToEdit_editableEntity()->getVisual3D()->peekRootDrawable_rotation() };
			qRoodDrawableRot.inverse();
			rot.mul(qRoodDrawableRot);

			rot.mul(rot1);
			rot.mul(rot2);
			
			// Remove the introduced rotation helper.
			Quaternion qRoodDrawableRot2{ *entityEditor->getEntityToEdit_editableEntity()->getVisual3D()->peekRootDrawable_rotation() };
			rot.mul(qRoodDrawableRot2);
			
			rot.mul(*entityEditor->getEntityToEdit_editableEntity()->getEditorStructure_rotator()->getRotation());

			entityEditor->getEntityToEdit_editableEntity()->getEditorStructure_rotator()->setRotation(rot);
		}
	}
}

TEE_Rotate::~TEE_Rotate() {
	//void
}
