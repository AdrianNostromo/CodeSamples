#include "TEEPl_Tool.h"
#include <graphics\visual3d\util\VoxelSides.h>
#include <rpg3D/gw/entity/module/voxelizedEditor/IVoxelizedEditorM_Edit.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/math/collision/Ray.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <base/list/Array3D.h>
#include <graphics/visual3d/drawable/VoxlesGrid3DGeometry.h>
#include <graphics/visual3d/drawable/VoxelGridDrawable3D.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <base/app/IApp.h>
#include <base/view/dialog/IDialogView.h>
#include <base/viewsHandler/IViewsHandler.h>

using namespace rpg3D;

const int TEEPl_Tool::TouchMode_TOOL = getNew_touchModeIncrementor();

const int TEEPl_Tool::VOXEL_MODEL_GROUP_SIZE_LIMIT = 20;

TEEPl_Tool::TEEPl_Tool(ToolConfigEEPlacer* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData)
{
	dialogEventListener.cb_onDynamicButtonActivated = std::bind(&TEEPl_Tool::dialog_onDynamicButtonActivated, this, std::placeholders::_1, std::placeholders::_2);
	dialogEventListener.cb_onCloseRequested = std::bind(&TEEPl_Tool::dialog_onCloseRequested, this, std::placeholders::_1);
	dialogEventListener.cb_onContentAreaTouched = std::bind(&TEEPl_Tool::dialog_onContentAreaTouched, this, std::placeholders::_1);
}

void TEEPl_Tool::dialog_onDynamicButtonActivated(base::IDialogView * view, base::DynamicTextButtonConfig * dynamicButtonConfig) {
	if (view != dialogView) {
		throw LogicException(LOC);
	}

	// The dialog has no buttons.
	throw LogicException(LOC);
}

void TEEPl_Tool::dialog_onCloseRequested(base::IDialogView * view) {
	if (view != dialogView) {
		throw LogicException(LOC);
	}

	dialogView = nullptr;
	view->hide(true, nullptr);
}

void TEEPl_Tool::dialog_onContentAreaTouched(base::IDialogView * view) {
	if (view != dialogView) {
		throw LogicException(LOC);
	}

	dialogView = nullptr;
	view->hide(true, nullptr);
}

void TEEPl_Tool::onEntityChangePre() {
	super::onEntityChangePre();

	if (entity == nullptr) {
		//void
	} else {
		//void
	}

	if (dialogView != nullptr) {
		dialogView->hide(true, nullptr);
		dialogView = nullptr;
	}
}

int TEEPl_Tool::computeTouchModeUnderPoint(float screenX, float screenY) {
	Ray& ray = entityEditor->getCinematicController()->peekCamera()->getPickRay(screenX, screenY, true);
	std::shared_ptr<IEditableVoxelisedM_Edit::TouchVoxelTarget> touchedVoxelResult = entityEditor->getEntityToEdit_editableEntity()->getClosestVoxelForRay(ray, false);
	if (touchedVoxelResult != nullptr) {
		return TouchMode_TOOL;
	}

	return super::computeTouchModeUnderPoint(screenX, screenY);
}

void TEEPl_Tool::onIsOperatingChanged(bool isOperating) {
	super::onIsOperatingChanged(isOperating);

	if (!isOperating) {
		// Do clearing on the deactivation because doing it on activation may cause problems with certain activation logics.
		voxelsPlacedOnCurrentDrag = 0;
	}
}

void TEEPl_Tool::tickActiveTouchMode(
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
		if (voxelsPlacedOnCurrentDrag < 1) {
			Ray& ray = entityEditor->getCinematicController()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
			std::shared_ptr<IEditableVoxelisedM_Edit::TouchVoxelTarget> touchedVoxelResult = entityEditor->getEntityToEdit_editableEntity()->getClosestVoxelForRay(ray, true);

			// Make sure the paint focus is on the entity voxels surface and not besides the entity.
			if (touchedVoxelResult != nullptr) {
				int gOffsetX = 0;
				int gOffsetY = 0;
				int gOffsetZ = 0;

				if (touchedVoxelResult->touchVoxelSide == VoxelSides::xMin) {
					gOffsetX -= 1;
				} else if (touchedVoxelResult->touchVoxelSide == VoxelSides::xMax) {
					gOffsetX += 1;
				} else if (touchedVoxelResult->touchVoxelSide == VoxelSides::yMin) {
					gOffsetY -= 1;
				} else if (touchedVoxelResult->touchVoxelSide == VoxelSides::yMax) {
					gOffsetY += 1;
				} else if (touchedVoxelResult->touchVoxelSide == VoxelSides::zMin) {
					gOffsetZ -= 1;
				} else if (touchedVoxelResult->touchVoxelSide == VoxelSides::zMax) {
					gOffsetZ += 1;
				} else {
					// This must always be valid.
					throw LogicException(LOC);
				}

				Array3D<VoxlesGrid3DGeometry::Voxel>* voxelsGrid = entityEditor->getEntityToEdit_editableEntity()->peekEditingVoxelsVisual()->peekVoxelsGrid()->peekGrid();
				tmp_gridAreaBounds.set(
					0, voxelsGrid->getCountA(),
					0, voxelsGrid->getCountB(),
					0, voxelsGrid->getCountC()
				)->unionA(
					touchedVoxelResult->gPos.x + gOffsetX, (touchedVoxelResult->gPos.x + gOffsetX) + 1,
					touchedVoxelResult->gPos.y + gOffsetY, (touchedVoxelResult->gPos.y + gOffsetY) + 1,
					touchedVoxelResult->gPos.z + gOffsetZ, (touchedVoxelResult->gPos.z + gOffsetZ) + 1
				);

				if (tmp_gridAreaBounds.getLenX() > VOXEL_MODEL_GROUP_SIZE_LIMIT
					|| tmp_gridAreaBounds.getLenY() > VOXEL_MODEL_GROUP_SIZE_LIMIT
					|| tmp_gridAreaBounds.getLenZ() > VOXEL_MODEL_GROUP_SIZE_LIMIT)
				{
					// Show a warning dialog.
					if (dialogView != nullptr) {
						dialogView->hide(true, nullptr);
						dialogView = nullptr;
					}
					dialogView = entityEditor->getEntity()->getHandler()->getHandler()->getApp()->getComponentAs<base::IViewsHandler*>(true/*mustExist*/)->getNewDialogViewAutoDisposable();
					dialogView->setListener(&dialogEventListener); 
					dialogView->show(
						true, nullptr,
						std::make_shared<base::IDialogView::Config>(
							250.0f /*widthDPM*/,
							std::make_shared<std::string>("Not possible") /*titleText*/, 1 /*titleTotalLinesCount*/,
							std::make_shared<base::IDialogView::DynamicTextItem>("Size limit of " + std::to_string(VOXEL_MODEL_GROUP_SIZE_LIMIT) + " reached.", 2) /*dynamicBodyTextItem*/, false /*isOneButtonControlPerLine*/,
							nullptr /*dynamicButtonConfigs*/
							)
					);
					
					// Stop the current tool.
					stopCurrentTouchMode();
				} else {
					entityEditor->getEntityToEdit_editableEntity()->setVoxelMaterialIndex(
						touchedVoxelResult->gPos.x + gOffsetX, touchedVoxelResult->gPos.y + gOffsetY, touchedVoxelResult->gPos.z + gOffsetZ,
						entityEditor->getEntityToEdit_editableEntity()->peekActiveDynamicMaterialIndex(),
						true
					);
				}
			}

			voxelsPlacedOnCurrentDrag++;
		}
	}
}

void TEEPl_Tool::disposeMain() {
	if (dialogView != nullptr) {
		throw LogicException(LOC);
	}

	super::disposeMain();
}

TEEPl_Tool::~TEEPl_Tool() {
	//void
}
