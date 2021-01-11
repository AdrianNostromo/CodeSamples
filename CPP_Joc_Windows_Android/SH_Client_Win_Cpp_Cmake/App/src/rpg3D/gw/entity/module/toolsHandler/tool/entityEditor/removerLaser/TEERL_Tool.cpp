#include "TEERL_Tool.h"
#include <rpg3D/gw/entity/module/voxelizedEditor/IVoxelizedEditorM_Edit.h>
#include <rpg3D/gw/entity/module/editableVoxelised/IEditableVoxelisedM_Edit.h>
#include <base/math/collision/Ray.h>
#include <graphics/environment/environment3D/cinematic/ICinematicController.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <base/app/IApp.h>
#include <base/view/dialog/IDialogView.h>
#include <base/viewsHandler/IViewsHandler.h>

using namespace rpg3D;

const int TEERL_Tool::TouchMode_TOOL = getNew_touchModeIncrementor();

TEERL_Tool::TEERL_Tool(ToolConfigEERemoverLaser* config, std::shared_ptr<ExtraData> extraData)
	: super(config, extraData)
{
	dialogEventListener.cb_onDynamicButtonActivated = std::bind(&TEERL_Tool::dialog_onDynamicButtonActivated, this, std::placeholders::_1, std::placeholders::_2);
	dialogEventListener.cb_onCloseRequested = std::bind(&TEERL_Tool::dialog_onCloseRequested, this, std::placeholders::_1);
	dialogEventListener.cb_onContentAreaTouched = std::bind(&TEERL_Tool::dialog_onContentAreaTouched, this, std::placeholders::_1);
}

void TEERL_Tool::dialog_onDynamicButtonActivated(base::IDialogView* view, base::DynamicTextButtonConfig* dynamicButtonConfig) {
	if (view != dialogView) {
		throw LogicException(LOC);
	}

	// The dialog has no buttons.
	throw LogicException(LOC);
}

void TEERL_Tool::dialog_onCloseRequested(base::IDialogView* view) {
	if (view != dialogView) {
		throw LogicException(LOC);
	}

	dialogView = nullptr;
	view->hide(true, nullptr);
}

void TEERL_Tool::dialog_onContentAreaTouched(base::IDialogView* view) {
	if (view != dialogView) {
		throw LogicException(LOC);
	}

	dialogView = nullptr;
	view->hide(true, nullptr);
}

void TEERL_Tool::onEntityChangePre() {
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

int TEERL_Tool::computeTouchModeUnderPoint(float screenX, float screenY) {
	Ray& ray = entityEditor->getCinematicController()->peekCamera()->getPickRay(screenX, screenY, true);
	std::shared_ptr<IEditableVoxelisedM_Edit::TouchVoxelTarget> touchedVoxelResult = entityEditor->getEntityToEdit_editableEntity()->getClosestVoxelForRay(ray, false);
	if (touchedVoxelResult != nullptr) {
		return TouchMode_TOOL;
	}

	return super::computeTouchModeUnderPoint(screenX, screenY);
}

void TEERL_Tool::onIsOperatingChanged(bool isOperating) {
	super::onIsOperatingChanged(isOperating);

	if (!isOperating) {
		// Do clearing on the deactivation because doing it on activation may cause problems with certain activation logics.
		voxelsRemovedOnCurrentDrag = 0;
	}
}

void TEERL_Tool::tickActiveTouchMode(
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
		std::shared_ptr<IEditableVoxelisedM_Edit::TouchVoxelTarget> touchedVoxelResult;
		Ray& ray = entityEditor->getCinematicController()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
		
		touchedVoxelResult = entityEditor->getEntityToEdit_editableEntity()->getClosestVoxelForRay(ray, false);

		// Make sure the paint focus is on the entity voxels surface and not besides the entity.
		if (touchedVoxelResult != nullptr) {
			if (entityEditor->getEntityToEdit_editableEntity()->isEditingEntitySingleVoxelRemaining()) {
				// Not allowed to remove the last voxel.
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
						std::make_shared<base::IDialogView::DynamicTextItem>("One does not delete all voxels.", 2) /*dynamicBodyTextItem*/, false /*isOneButtonControlPerLine*/,
						nullptr /*dynamicButtonConfigs*/
						)
				);
				
				// Stop the current tool. This will remove 1 voxel per frame and look better.
				stopCurrentTouchMode();
			} else {
				entityEditor->getEntityToEdit_editableEntity()->setVoxelMaterialIndex(
					touchedVoxelResult->gPos.x, touchedVoxelResult->gPos.y, touchedVoxelResult->gPos.z,
					-1,
					true
				);

				voxelsRemovedOnCurrentDrag++;
			}
		}
	}
}

void TEERL_Tool::disposeMain() {
	if (dialogView != nullptr) {
		throw LogicException(LOC);
	}

	super::disposeMain();
}

TEERL_Tool::~TEERL_Tool() {
	//void
}
