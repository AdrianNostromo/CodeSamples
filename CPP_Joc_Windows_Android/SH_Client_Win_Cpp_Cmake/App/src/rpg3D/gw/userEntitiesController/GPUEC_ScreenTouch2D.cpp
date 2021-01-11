#include "GPUEC_ScreenTouch2D.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <base/input/util/TouchEventType.h>
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <base/math/Math.h>
#include <base/screen/ScreenUtil.h>
#include <rpg3D/gw/entity/module/userControlled/util/IWorldOrEntityInteractionFilter.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <worldGame3D/gw/interaction/IGPInteraction.h>
#include <rpg3D/gw/interaction/deviceinput/customSensor/SensorDataLocalisedScreenCursor.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include <graphics/camera/Camera.h>
#include <worldGame3D/gw/sim/IGPSim.h>
#include <rpg3D/gw/entity/module/userControlled/filters/IWorldInteractionFilter.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <base/app/IAppMetrics.h>
#include <rpg3D/gw/interaction/deviceinput/MDIN_LockedCursor.h>

using namespace rpg3D;

const float GPUEC_ScreenTouch2D::CursorHandlingData::AUTO_REMOVE_TIMER_S = 12.0f;

GPUEC_ScreenTouch2D::GPUEC_ScreenTouch2D(base::IGameWorld* gw)
	: super(gw)
{
    //void
}

void GPUEC_ScreenTouch2D::createMain() {
	super::createMain();

	screen = gw->getHandler()->getAppMetrics()->getScreen();
}

GPUEC_ScreenTouch2D::CursorHandlingData* GPUEC_ScreenTouch2D::getOrCreateCursorHandlingData(int cursorIndex) {
	// Use nullptr padding.
	while (cursorIndex > cursorHandlingDataList.count()) {
		cursorHandlingDataList.appendDirect(nullptr);
	}

	if (cursorIndex >= cursorHandlingDataList.count()) {
		cursorHandlingDataList.appendDirect(std::make_shared<CursorHandlingData>(cursorIndex));
	}else if(cursorHandlingDataList.getDirect(cursorIndex) == nullptr) {
		cursorHandlingDataList.setDirect(cursorIndex, std::make_shared<CursorHandlingData>(cursorIndex));
	}
	
	std::shared_ptr<CursorHandlingData>* cursorHD = cursorHandlingDataList.getPointer(cursorIndex);
	if (cursorHD == nullptr) {
		// Should have been created.
		throw LogicException(LOC);
	}

	return cursorHD->get();
}


bool GPUEC_ScreenTouch2D::processCacheInput(std::shared_ptr<base::SensorEvent>& input) {
	bool ret = super::processCacheInput(input);

	if (controledEntityTarget->entity == nullptr) {
		return ret;
	}

	if (input->sensorType == IManagedDeviceInput::CustomSensorTypes::LocalisedScreenCursor) {
		ret = true;

		rpg3D::SensorDataLocalisedScreenCursor* cSensorData = input->data->castToSubtype<rpg3D::SensorDataLocalisedScreenCursor*>();

		CursorHandlingData* cursorHD = getOrCreateCursorHandlingData(cSensorData->cursorIndex);
		if (cursorHD->cacheScreenPos == nullptr) {
			cursorHD->cacheScreenPos = std::make_shared<Vector2>(cSensorData->screenPos);
		}else {
			cursorHD->cacheScreenPos->set(cSensorData->screenPos);
		}

		if (cSensorData->touchEventType == TouchEventType::CURSOR_DOWN) {
			if (cSensorData->buttonId == base::Touch::ButtonCode::LeftOrTouch) {
				if (cursorHD->cursorIndex < 0) {
					// This can occur if the input entity is changed from a input filter.
					throw LogicException(LOC);
				}

				processAction_cursorDown_Main(cursorHD, cSensorData->buttonId, cSensorData->touchEventType, cSensorData->screenPos);

				dispatchWorldInteractionFilters(cursorHD, cSensorData->buttonId, cSensorData->touchEventType, cSensorData->screenPos);
			}else if (cSensorData->buttonId == base::Touch::ButtonCode::Right) {
				processAction_cursorDown_Alternative(cursorHD, cSensorData->screenPos);

				dispatchWorldInteractionFilters(cursorHD, cSensorData->buttonId, cSensorData->touchEventType, cSensorData->screenPos);
			}else {
				throw LogicException(LOC);
			}
		}else if (cSensorData->touchEventType == TouchEventType::CURSOR_UP || cSensorData->touchEventType == TouchEventType::CURSOR_UP_OUTSIDE) {
			if (cSensorData->buttonId == base::Touch::ButtonCode::LeftOrTouch) {
				processAction_cursorUp_Main(cSensorData->cursorIndex, cSensorData->screenPos);
			}else if (cSensorData->buttonId == base::Touch::ButtonCode::Right) {
				processAction_cursorUp_Alternative(cSensorData->cursorIndex, cSensorData->screenPos);
			}else {
				throw LogicException(LOC);
			}

			// Use CURSOR_UP because CURSOR_UP_OUTSIDE is not "known" by the WIF logic.
			dispatchWorldInteractionFilters(cursorHD, cSensorData->buttonId, TouchEventType::CURSOR_UP, cSensorData->screenPos);
		}else if (cSensorData->touchEventType == TouchEventType::CURSOR_MOVE) {
			CursorHandlingData* cursorHD = getOrCreateCursorHandlingData(cSensorData->cursorIndex);

			cursorHD->onData_move(cSensorData->screenPos);
		}else if (cSensorData->touchEventType == TouchEventType::CURSOR_DRAG) {
			if (cSensorData->buttonId == base::Touch::ButtonCode::LeftOrTouch) {
				CursorHandlingData* cursorHD = getOrCreateCursorHandlingData(cSensorData->cursorIndex);

				cursorHD->onData_drag_Main(cSensorData->screenPos);
			}else if (cSensorData->buttonId == base::Touch::ButtonCode::Right) {
				CursorHandlingData* cursorHD = getOrCreateCursorHandlingData(cSensorData->cursorIndex);

				cursorHD->onData_drag_Alternative(cSensorData->screenPos);
			}else {
				throw LogicException(LOC);
			}
		}else {
			throw LogicException(LOC);
		}
	}

	return ret;
}

//asd_tool;// Modify to move each filter to a class;
//asd_tool;// The requesters push filter objects that are saved in a list and have a source_input_type (eg. cursor_move, down, button_down, ...);

void GPUEC_ScreenTouch2D::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_localInput(deltaS, deltaMS, gameTimeS);

	for (int cursorIndex = 0; cursorIndex < cursorHandlingDataList.count(); cursorIndex++) {
		std::shared_ptr<CursorHandlingData>& cursorHD = cursorHandlingDataList.getReference(cursorIndex);

		if (cursorHD != nullptr) {
			// After some time (eg. ~=12s) mark the cursor as iddle and remove it from the list.
			/// This is done to avoid events spamming.
			cursorHD->autoRemoveTimerS -= deltaS;
			if (cursorHD->autoRemoveTimerS <= 0.0f) {
				// Cursor is iddle, remove it.
				cursorHandlingDataList.setDirect(cursorIndex, nullptr);
				while (cursorHandlingDataList.count() > 0 && cursorHandlingDataList.getDirect(cursorHandlingDataList.count() - 1) == nullptr) {
					cursorHandlingDataList.remove(cursorHandlingDataList.count() - 1);
				}

				continue;
			}

			onGameLoop_move(cursorHD.get());
			onGameLoop_drag_Main(cursorHD.get());
			onGameLoop_drag_Alternative(cursorHD.get());
		}
	}
}

void GPUEC_ScreenTouch2D::onGameLoop_move(CursorHandlingData* cursorHD) {
	// Update on camera_change, unit_pos_change, data, force_update.
	bool isDirty_newData = false;
	bool isDirty_forceUpdate = false;
	bool isDirty_cameraUpdate = false;
	bool isDirty_unitPos = false;

	int cameraUpdateIndex = gw->getMain()->getEnvironment3D()->peekCamera()->getUpdateIndex();
	Vector3* unitPos = controledEntityTarget->entity->getPos();

	if (cursorHD->isData_move) {
		// New data.
		isDirty_newData = true;

		cursorHD->isData_move = false;
	}
	if(isDirty_newData || (!localCache_isCursorBlockedMode && cursorHD->lastDispatched_move_screenPos != nullptr)) {
		// There is updating_now or was_a_initial_update and normal_cursor_mode.

		if (isDirtyControlType) {
			// Control type changed.
			isDirty_forceUpdate = true;
		}
		if (cursorHD->lastDispatched_move_cameraUpdateIndex != cameraUpdateIndex) {
			// Camera changed.
			isDirty_cameraUpdate = true;
		}
		if(cursorHD->lastDispatched_move_unitPos == nullptr || !cursorHD->lastDispatched_move_unitPos->equals(unitPos)) {
			// Unit pos changed.
			isDirty_unitPos = true;
		}
	}

	if (!localCache_isCursorBlockedMode && (isDirty_newData || cursorHD->cacheScreenPos != nullptr) && (isDirty_newData || isDirty_forceUpdate || isDirty_cameraUpdate || isDirty_unitPos)) {
		Vector2* newScreenPos = isDirty_newData ? &cursorHD->data_move_screenPos : cursorHD->cacheScreenPos.get();
		
		Vector2 mousePosDelta{};
		Vector2 mousePosDelta_sensitised{};
		if (cursorHD->lastDispatched_move_screenPos != nullptr) {
			mousePosDelta.set(
				newScreenPos->x - cursorHD->lastDispatched_move_screenPos->x,
				newScreenPos->y - cursorHD->lastDispatched_move_screenPos->y
			);
			
			Vector2* screenDensityMultiplier = screen->getScreenDensityMultiplier();
			mousePosDelta_sensitised.set(
				(mousePosDelta.x * rpg3D::MDIN_LockedCursor::mouseSensitivityMultiplier) / screenDensityMultiplier->x,
				(mousePosDelta.y * rpg3D::MDIN_LockedCursor::mouseSensitivityMultiplier) / screenDensityMultiplier->y
			);
		}else {
			mousePosDelta.set(0.0f, 0.0f);
			mousePosDelta_sensitised.set(0.0f, 0.0f);
		}

		tick_cursorMove(
			isDirty_newData, isDirty_forceUpdate, isDirty_cameraUpdate, isDirty_unitPos,
			cursorHD, cursorHD->cursorIndex, *newScreenPos, mousePosDelta, mousePosDelta_sensitised
		);

		if (cursorHD->lastDispatched_move_screenPos == nullptr) {
			cursorHD->lastDispatched_move_screenPos = std::make_shared<Vector2>(*newScreenPos);
		}else {
			cursorHD->lastDispatched_move_screenPos->set(*newScreenPos);
		}
		if (cursorHD->lastDispatched_move_unitPos == nullptr) {
			cursorHD->lastDispatched_move_unitPos = std::make_shared<Vector3>(unitPos);
		}else {
			cursorHD->lastDispatched_move_unitPos->set(*unitPos);
		}
		cursorHD->lastDispatched_move_cameraUpdateIndex = cameraUpdateIndex;
	}
}

void GPUEC_ScreenTouch2D::onGameLoop_drag_Main(CursorHandlingData* cursorHD) {
	// Update on camera_change, unit_pos_change, data, force_update.
	bool isDirty_newData = false;
	bool isDirty_forceUpdate = false;
	bool isDirty_cameraUpdate = false;
	bool isDirty_unitPos = false;

	int cameraUpdateIndex = gw->getMain()->getEnvironment3D()->peekCamera()->getUpdateIndex();
	Vector3* unitPos = controledEntityTarget->entity->getPos();

	if (cursorHD->isData_drag_Main) {
		// New data.
		isDirty_newData = true;

		cursorHD->isData_drag_Main = false;
	}
	if(isDirty_newData || (!localCache_isCursorBlockedMode && cursorHD->lastDispatched_drag_Main_screenPos != nullptr)) {
		// There is updating_now or was_a_initial_update and normal_cursor_mode.

		if (isDirtyControlType) {
			// Control type changed.
			isDirty_forceUpdate = true;
		}
		if (cursorHD->lastDispatched_drag_Main_cameraUpdateIndex != cameraUpdateIndex) {
			// Camera changed.
			isDirty_cameraUpdate = true;
		}
		if(cursorHD->lastDispatched_drag_Main_unitPos == nullptr || !cursorHD->lastDispatched_drag_Main_unitPos->equals(unitPos->x, unitPos->y, unitPos->z, Math::FLOAT_ROUNDING_ERROR)) {
			// Unit pos changed.
			isDirty_unitPos = true;
		}
	}

	if (!localCache_isCursorBlockedMode && (isDirty_newData || cursorHD->cacheScreenPos != nullptr) && (isDirty_newData || isDirty_forceUpdate || isDirty_cameraUpdate || isDirty_unitPos)) {
		Vector2* newScreenPos = isDirty_newData ? &cursorHD->data_drag_Main_screenPos : cursorHD->cacheScreenPos.get();
		
		Vector2 mousePosDelta{};
		Vector2 mousePosDelta_sensitised{};
		if (cursorHD->lastDispatched_drag_Main_screenPos != nullptr) {
			mousePosDelta.set(
				newScreenPos->x - cursorHD->lastDispatched_drag_Main_screenPos->x,
				newScreenPos->y - cursorHD->lastDispatched_drag_Main_screenPos->y
			);
			
			Vector2* screenDensityMultiplier = screen->getScreenDensityMultiplier();
			mousePosDelta_sensitised.set(
				(mousePosDelta.x * rpg3D::MDIN_LockedCursor::mouseSensitivityMultiplier) / screenDensityMultiplier->x,
				(mousePosDelta.y * rpg3D::MDIN_LockedCursor::mouseSensitivityMultiplier) / screenDensityMultiplier->y
			);
		}else {
			mousePosDelta.set(0.0f, 0.0f);
			mousePosDelta_sensitised.set(0.0f, 0.0f);
		}

		tick_cursorDrag_Main(
			isDirty_newData, isDirty_forceUpdate, isDirty_cameraUpdate, isDirty_unitPos,
			cursorHD, cursorHD->cursorIndex, *newScreenPos, mousePosDelta, mousePosDelta_sensitised
		);

		if (cursorHD->lastDispatched_drag_Main_screenPos == nullptr) {
			cursorHD->lastDispatched_drag_Main_screenPos = std::make_shared<Vector2>(*newScreenPos);
		}else {
			cursorHD->lastDispatched_drag_Main_screenPos->set(*newScreenPos);
		}
		if (cursorHD->lastDispatched_drag_Main_unitPos == nullptr) {
			cursorHD->lastDispatched_drag_Main_unitPos = std::make_shared<Vector3>(unitPos);
		}else {
			cursorHD->lastDispatched_drag_Main_unitPos->set(*unitPos);
		}
		cursorHD->lastDispatched_drag_Main_cameraUpdateIndex = cameraUpdateIndex;
	}
}

void GPUEC_ScreenTouch2D::onGameLoop_drag_Alternative(CursorHandlingData* cursorHD) {
	// Update on camera_change, unit_pos_change, data, force_update.
	bool isDirty_newData = false;
	bool isDirty_forceUpdate = false;
	bool isDirty_cameraUpdate = false;
	bool isDirty_unitPos = false;

	int cameraUpdateIndex = gw->getMain()->getEnvironment3D()->peekCamera()->getUpdateIndex();
	Vector3* unitPos = controledEntityTarget->entity->getPos();

	if (cursorHD->isData_drag_Alternative) {
		// New data.
		isDirty_newData = true;

		cursorHD->isData_drag_Alternative = false;
	}
	if(isDirty_newData || (!localCache_isCursorBlockedMode && cursorHD->lastDispatched_drag_Alternative_screenPos != nullptr)) {
		// There is updating_now or was_a_initial_update and normal_cursor_mode.

		if (isDirtyControlType) {
			// Control type changed.
			isDirty_forceUpdate = true;
		}
		if (cursorHD->lastDispatched_drag_Alternative_cameraUpdateIndex != cameraUpdateIndex) {
			// Camera changed.
			isDirty_cameraUpdate = true;
		}
		if(cursorHD->lastDispatched_drag_Alternative_unitPos == nullptr || !cursorHD->lastDispatched_drag_Alternative_unitPos->equals(unitPos)) {
			// Unit pos changed.
			isDirty_unitPos = true;
		}
	}

	if (!localCache_isCursorBlockedMode && (isDirty_newData || cursorHD->cacheScreenPos != nullptr) && (isDirty_newData || isDirty_forceUpdate || isDirty_cameraUpdate || isDirty_unitPos)) {
		Vector2* newScreenPos = isDirty_newData ? &cursorHD->data_drag_Alternative_screenPos : cursorHD->cacheScreenPos.get();
		
		Vector2 mousePosDelta{};
		Vector2 mousePosDelta_sensitised{};
		if (cursorHD->lastDispatched_drag_Alternative_screenPos != nullptr) {
			mousePosDelta.set(
				newScreenPos->x - cursorHD->lastDispatched_drag_Alternative_screenPos->x,
				newScreenPos->y - cursorHD->lastDispatched_drag_Alternative_screenPos->y
			);

			Vector2* screenDensityMultiplier = screen->getScreenDensityMultiplier();
			mousePosDelta_sensitised.set(
				(mousePosDelta.x * rpg3D::MDIN_LockedCursor::mouseSensitivityMultiplier) / screenDensityMultiplier->x,
				(mousePosDelta.y * rpg3D::MDIN_LockedCursor::mouseSensitivityMultiplier) / screenDensityMultiplier->y
			);
		} else {
			mousePosDelta.set(0.0f, 0.0f);
			mousePosDelta_sensitised.set(0.0f, 0.0f);
		}

		tick_cursorDrag_Alternative(
			isDirty_newData, isDirty_forceUpdate, isDirty_cameraUpdate, isDirty_unitPos,
			cursorHD, cursorHD->cursorIndex, *newScreenPos, mousePosDelta, mousePosDelta_sensitised
		);

		if (cursorHD->lastDispatched_drag_Alternative_screenPos == nullptr) {
			cursorHD->lastDispatched_drag_Alternative_screenPos = std::make_shared<Vector2>(*newScreenPos);
		}else {
			cursorHD->lastDispatched_drag_Alternative_screenPos->set(*newScreenPos);
		}
		if (cursorHD->lastDispatched_drag_Alternative_unitPos == nullptr) {
			cursorHD->lastDispatched_drag_Alternative_unitPos = std::make_shared<Vector3>(unitPos);
		}else {
			cursorHD->lastDispatched_drag_Alternative_unitPos->set(*unitPos);
		}
		cursorHD->lastDispatched_drag_Alternative_cameraUpdateIndex = cameraUpdateIndex;
	}
}

void GPUEC_ScreenTouch2D::dispatchWorldInteractionFilters(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId, TouchEventType* touchType, Vector2& screenPos) {
	int cameraModeBitGroup = gw->getSim()->getCameraMode();
	int blockingPriorityValue = -1;

	ListDL<IWorldInteractionFilter*>* listToEnumerate = nullptr;
	if (buttonId == base::Touch::ButtonCode::LeftOrTouch) {
		if (touchType == TouchEventType::CURSOR_DOWN) {
			listToEnumerate = &customFiltersLists_ScreenCursor_Main_Down;
		} else if (touchType == TouchEventType::CURSOR_UP) {
			listToEnumerate = &customFiltersLists_ScreenCursor_Main_Up;
		} else if (touchType == TouchEventType::CURSOR_DRAG) {
			listToEnumerate = &customFiltersLists_ScreenCursor_Main_Drag;
		}
	} else if (buttonId == base::Touch::ButtonCode::Right) {
		if (touchType == TouchEventType::CURSOR_DOWN) {
			listToEnumerate = &customFiltersLists_ScreenCursor_Alt_Down;
		} else if (touchType == TouchEventType::CURSOR_UP) {
			listToEnumerate = &customFiltersLists_ScreenCursor_Alt_Up;
		} else if (touchType == TouchEventType::CURSOR_DRAG) {
			listToEnumerate = &customFiltersLists_ScreenCursor_Alt_Drag;
		}
	}

	if (listToEnumerate != nullptr) {
		listToEnumerate->enumerateDirect([&screenPos, this, cursorHD, cameraModeBitGroup, &blockingPriorityValue, buttonId, touchType](ListDL<IWorldInteractionFilter*>::Entry* listEntry, IWorldInteractionFilter*& data, bool& stopEnumeration)->void {
			if (blockingPriorityValue > data->getInputPriority()) {
				// Priority blocking was set and is higher than the current entry.
				stopEnumeration = true;
				return;
			}

			if ((cameraModeBitGroup & data->getCameraModesBitMaskAny()) != 0) {
				bool isEventUsed = data->onCursorInput(
					base::Touch::CursorMode::Normal, buttonId, touchType,
					cursorHD->cursorIndex, screenPos,
					controledEntityTarget->entity, cameraModeBitGroup, gw
				);
				if (isEventUsed) {
					blockingPriorityValue = data->getInputPriority();
				}
			}
		});
	}
}

void GPUEC_ScreenTouch2D::processAction_cursorDown_Main(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId, TouchEventType* touchType, Vector2& screenPos) {
	if (cursorHD->lastDispatched_drag_Main_screenPos == nullptr) {
		cursorHD->lastDispatched_drag_Main_screenPos = std::make_shared<Vector2>(screenPos);
	}else {
		cursorHD->lastDispatched_drag_Main_screenPos->set(screenPos);
	}

	filterTriggerEventIndexCounter++;

	// Do entity down before ground down;
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::EntityDown, false);
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcherGlobal = globalInteractionsDispatcher.getDispatcher(IWorldOrEntityInteractionFilter::Type::EntityDown, false);
	if ((dispatcher != nullptr && dispatcher->getListenersCount() > 0) || (dispatcherGlobal != nullptr && dispatcherGlobal->getListenersCount() > 0)) {
		// Compute interaction info.
		Ray& ray = gw->getMain()->getEnvironment3D()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
		std::shared_ptr<ArrayList<base::IGamePartMain::DT_Entity_Distance>> touchedEntitiesList = gw->getGPMain()->getEntitiesUnderTouch(ray, nullptr);

		if (touchedEntitiesList != nullptr && touchedEntitiesList->count() > 0) {
			// Do manual dispatch and check the filter.
			if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
				dispatchInteraction_Filtered_Entity(
					controledEntityTarget->entity, touchedEntitiesList,
					IWorldOrEntityInteractionFilter::Type::EntityDown, filterTriggerEventIndexCounter, cursorHD->cursorIndex,
					dispatcher
				);
			}
			if (dispatcherGlobal != nullptr && dispatcherGlobal->getListenersCount() > 0) {
				dispatchInteraction_Filtered_Entity(
					nullptr/*entity*/, touchedEntitiesList,
					IWorldOrEntityInteractionFilter::Type::EntityDown, filterTriggerEventIndexCounter, cursorHD->cursorIndex,
					dispatcherGlobal
				);
			}
		}
	}

	dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::GroundDown, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		// Compute interaction info.
		Vector3 groundPos{};
		gw->getMain()->camera2DCoordsProjectionTo3DGroundPos(
			screenPos/*screenPos*/,
			0.0f/*groundPlanePosZ*/,
			&groundPos/*oIntersectionPos*/,
			nullptr/*oRayDirection*/
		);

		// Ground always dispatches, there is not filter test required.
		dispatchInteraction_Filtered_WorldPos(
			controledEntityTarget->entity,
			rpg3D::WorldOrEntityInteractionEvent::getNew_GroundDown(filterTriggerEventIndexCounter, cursorHD->cursorIndex, groundPos),
			dispatcher, IWorldOrEntityInteractionFilter::Type::GroundDown
		);
	}

	processAction_cursorDown_Main_B(cursorHD, screenPos);

	dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Main, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		dispatchInteraction_NotFiltered(
			rpg3D::WorldOrEntityInteractionEvent::getNew_ScreenCursor_Down_Main(filterTriggerEventIndexCounter, cursorHD->cursorIndex, screenPos),
			dispatcher, IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Main
		);
	}
}

void GPUEC_ScreenTouch2D::processAction_cursorDown_Main_B(CursorHandlingData* cursorHD, Vector2& screenPos) {
	bool isDispatched_UnitFeetGroundPlanePos_Down_MainBtn = false;
	bool isRaySet = false;
	Vector3 rayDir{};

	int camMode = gw->getSim()->getCameraMode();
	
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher;
	if (camMode == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
		dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_1stPersonCamera_Down_MainBtn, false);
	} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
		dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_3rdPersonCamera_Down_MainBtn, false);
	} else {
		// Unhandled camera mode.
		throw LogicException(LOC);
	}

	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		// Compute interaction info.
		Vector3 unitFeetGroundPlanePos{};
		bool isIntersection = gw->getMain()->camera2DCoordsProjectionTo3DGroundPos(
			screenPos/*screenPos*/,
			controledEntityTarget->entity->getZ()/*groundPlanePosZ*/,
			&unitFeetGroundPlanePos/*oIntersectionPos*/,
			&rayDir/*oRayDirection*/
		);
		isRaySet = true;

		if (isIntersection) {
			isDispatched_UnitFeetGroundPlanePos_Down_MainBtn = true;

			if (camMode == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
				dispatchInteraction_Filtered_WorldPos(
					controledEntityTarget->entity,
					rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetGroundPlanePos_1stPersonCamera_Down_MainBtn(
						filterTriggerEventIndexCounter,
						cursorHD->cursorIndex,
						unitFeetGroundPlanePos
					),
					dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_1stPersonCamera_Down_MainBtn
				);
			} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
				dispatchInteraction_Filtered_WorldPos(
					controledEntityTarget->entity,
					rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetGroundPlanePos_3rdPersonCamera_Down_MainBtn(
						filterTriggerEventIndexCounter,
						cursorHD->cursorIndex,
						unitFeetGroundPlanePos
					),
					dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_3rdPersonCamera_Down_MainBtn
				);
			} else {
				// Unhandled camera mode.
				throw LogicException(LOC);
			}
		}

		// This is here so it won't dispatch if the above one has no listeners. THe above on must have listeners for this to be dispatchable.
		if (!isDispatched_UnitFeetGroundPlanePos_Down_MainBtn) {
			if (camMode == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
				dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_1stPersonCamera_Down_MainBtn, false);
			} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
				dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_3rdPersonCamera_Down_MainBtn, false);
			} else {
				// Unhandled camera mode.
				throw LogicException(LOC);
			}

			if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
				// Get the above checked or create a new ray.
				if (!isRaySet) {
					isRaySet = true;

					gw->getGPMain()->camera2DCoordsProjectionGetRayDir(
						screenPos/*screenPos*/,
						&rayDir/*oRayDirection*/
					);
				}

				// Ground always dispatches, there is not filter test required.
				int camMode = gw->getSim()->getCameraMode();
				if (camMode == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
					dispatchInteraction_NotFiltered(
						rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetHalfSphereDomeDir_1stPersonCamera_Down_MainBtn(
							filterTriggerEventIndexCounter,
							cursorHD->cursorIndex,
							rayDir
						),
						dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_1stPersonCamera_Down_MainBtn
					);
				} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
					dispatchInteraction_NotFiltered(
						rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetHalfSphereDomeDir_3rdPersonCamera_Down_MainBtn(
							filterTriggerEventIndexCounter,
							cursorHD->cursorIndex,
							rayDir
						),
						dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_3rdPersonCamera_Down_MainBtn
					);
				} else {
					// Unhandled camera mode.
					throw LogicException(LOC);
				}
			}
		}
	}
}

void GPUEC_ScreenTouch2D::processAction_cursorDown_Alternative(CursorHandlingData* cursorHD, Vector2& screenPos) {
	if (cursorHD->lastDispatched_drag_Alternative_screenPos == nullptr) {
		cursorHD->lastDispatched_drag_Alternative_screenPos = std::make_shared<Vector2>(screenPos);
	}else {
		cursorHD->lastDispatched_drag_Alternative_screenPos->set(screenPos);
	}

	filterTriggerEventIndexCounter++;

	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Alternative, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		dispatchInteraction_NotFiltered(
            rpg3D::WorldOrEntityInteractionEvent::getNew_ScreenCursor_Down_Alternative(filterTriggerEventIndexCounter, cursorHD->cursorIndex, screenPos),
            dispatcher, IWorldOrEntityInteractionFilter::Type::ScreenCursor_Down_Alternative
        );
	}
}

void GPUEC_ScreenTouch2D::tick_cursorMove(
	bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
	CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& mousePosDelta, Vector2& mousePosDelta_sensitised)
{
	// This function is called on many changes. Make individual checks.

	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher;

	// Also update in zero_move_dist because the action it'self is important, not the move amount.
	if (isDirty_forceUpdate || isDirty_newData) {
		dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Move, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_ScreenCursor_Move(
					-1, cursorIndex,
					screenPos, mousePosDelta, mousePosDelta_sensitised
				),
				dispatcher, IWorldOrEntityInteractionFilter::Type::ScreenCursor_Move
			);
		}
	}

	if (isDirty_forceUpdate || isDirty_newData || isDirty_cameraUpdate) {
		dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::GlobalFocusDirection2D, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			// Compute interaction info.
			Vector3 groundPos{};
			gw->getMain()->camera2DCoordsProjectionTo3DGroundPos(
				screenPos/*screenPos*/,
				0.0f/*groundPlanePosZ*/,
				&groundPos/*oIntersectionPos*/,
				nullptr/*oRayDirection*/
			);
			float angZRad = Math::atan2(groundPos.y - controledEntityTarget->entity->getY(), groundPos.x - controledEntityTarget->entity->getX());
			float angZDeg = Math::toDegrees(angZRad);
			if (isnan(angZDeg)) {
				// This occured when the window was minimised.
				throw LogicException(LOC);
			}

			// Ground always dispatches, there is not filter test required.
			bool isSource_cursor = isDirty_newData ? true : false;

			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_GlobalFocusDirection2D(-1, angZDeg, isSource_cursor),
				dispatcher, IWorldOrEntityInteractionFilter::Type::GlobalFocusDirection2D
			);
		}
	}

	if (isDirty_forceUpdate || isDirty_newData || isDirty_cameraUpdate || isDirty_unitPos) {
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::GlobalFocusDirection2DRelativeToUnit, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			// Compute interaction info.
			Vector3 groundPos{};
			gw->getMain()->camera2DCoordsProjectionTo3DGroundPos(
				screenPos/*screenPos*/,
				0.0f/*groundPlanePosZ*/,
				&groundPos/*oIntersectionPos*/,
				nullptr/*oRayDirection*/
			);
			float angZRad = Math::atan2(groundPos.y - controledEntityTarget->entity->getY(), groundPos.x - controledEntityTarget->entity->getX());
			float angZDeg = Math::toDegrees(angZRad);

			// Ground always dispatches, there is not filter test required.
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_GlobalFocusDirection2DRelativeToUnit(-1, angZDeg),
				dispatcher, IWorldOrEntityInteractionFilter::Type::GlobalFocusDirection2DRelativeToUnit
			);
		}
	}

	if (isDirty_forceUpdate || isDirty_newData || isDirty_cameraUpdate) {
		dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::GroundMove, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			// Compute interaction info.
			Vector3 groundPos{};
			gw->getMain()->camera2DCoordsProjectionTo3DGroundPos(
				screenPos/*screenPos*/,
				0.0f/*groundPlanePosZ*/,
				&groundPos/*oIntersectionPos*/,
				nullptr/*oRayDirection*/
			);

			// Ground always dispatches, there is not filter test required.
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_GroundMove(-1, cursorIndex, groundPos),
				dispatcher, IWorldOrEntityInteractionFilter::Type::GroundMove
			);
		}
	}
}

void GPUEC_ScreenTouch2D::tick_cursorDrag_Main(
	bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
	CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& mousePosDelta, Vector2& mousePosDelta_sensitised)
{
	if (isDirty_newData || isDirty_forceUpdate) {
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Main, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			// Ground always dispatches, there is not filter test required.
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_ScreenCursor_Drag_Main(
					filterTriggerEventIndexCounter, cursorIndex,
					screenPos, mousePosDelta, mousePosDelta_sensitised
				),
				dispatcher, IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Main
			);
		}
	}

	tick_cursorDrag_Main_B(
		isDirty_newData, isDirty_forceUpdate, isDirty_cameraUpdate, isDirty_unitPos,
		cursorHD, cursorIndex, screenPos, mousePosDelta);
}

void GPUEC_ScreenTouch2D::tick_cursorDrag_Main_B(
	bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
	CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& mousePosDelta)
{
	// Note. isDirty_forceUpdate is currently not checked, maybe no need to check it (not sure, works as it is).
	if (isDirty_newData || isDirty_cameraUpdate || (isDirty_unitPos && (cursorHD->lastDispatched_drag_Main_unitPos == nullptr || cursorHD->lastDispatched_drag_Main_unitPos->z != controledEntityTarget->entity->getZ()))) {
		bool isDispatched_UnitFeetGroundPlanePos_Drag_MainBtn = false;
		bool isRaySet = false;
		Vector3 rayDir{};

		int camMode = gw->getSim()->getCameraMode();

		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher;
		if (camMode == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
			dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_1stPersonCamera_Drag_MainBtn, false);
		} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
			dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_3rdPersonCamera_Drag_MainBtn, false);
		} else {
			// Unhandled camera mode.
			throw LogicException(LOC);
		}
		if (dispatcher != nullptr) {
			if (dispatcher->getListenersCount() > 0) {
				// Compute interaction info.
				Vector3 unitFeetGroundPlanePos{};
				bool isIntersection = gw->getMain()->camera2DCoordsProjectionTo3DGroundPos(
					screenPos,
					controledEntityTarget->entity->getZ(),
					&unitFeetGroundPlanePos,
					&rayDir
				);
				isRaySet = true;

				if (isIntersection) {
					isDispatched_UnitFeetGroundPlanePos_Drag_MainBtn = true;

					if (camMode == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
						dispatchInteraction_Filtered_WorldPos(
							controledEntityTarget->entity,
							rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetGroundPlanePos_1stPersonCamera_Drag_MainBtn(
								filterTriggerEventIndexCounter,
								cursorIndex,
								unitFeetGroundPlanePos
							),
							dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_1stPersonCamera_Drag_MainBtn
						);
					} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
						dispatchInteraction_Filtered_WorldPos(
							controledEntityTarget->entity,
							rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetGroundPlanePos_3rdPersonCamera_Drag_MainBtn(
								filterTriggerEventIndexCounter,
								cursorIndex,
								unitFeetGroundPlanePos
							),
							dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_3rdPersonCamera_Drag_MainBtn
						);
					} else {
						// Unhandled camera mode.
						throw LogicException(LOC);
					}
				}

				// This is here so it won't dispatch if the above one has no listeners. THe above on must have listeners for this to be dispatchable.
				// This dispatches only on drag, NOT on unit_move because the direction is always the same.
				if (!isDispatched_UnitFeetGroundPlanePos_Drag_MainBtn) {
					if (camMode == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
						dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_1stPersonCamera_Drag_MainBtn, false);
					} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
						dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_3rdPersonCamera_Drag_MainBtn, false);
					} else {
						// Unhandled camera mode.
						throw LogicException(LOC);
					}
					if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
						// Get the above checked or create a new ray.
						if (!isRaySet) {
							isRaySet = true;

							gw->getGPMain()->camera2DCoordsProjectionGetRayDir(
								screenPos,
								&rayDir
							);
						}

						// Ground always dispatches, there is not filter test required.
						if (camMode == GameCameraModeBitGroups::followerFirstPersonTargetRotation) {
							dispatchInteraction_NotFiltered(
								rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetHalfSphereDomeDir_1stPersonCamera_Drag_MainBtn(
									filterTriggerEventIndexCounter,
									cursorIndex,
									rayDir
								),
								dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_1stPersonCamera_Drag_MainBtn
							);
						} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
							dispatchInteraction_NotFiltered(
								rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetHalfSphereDomeDir_3rdPersonCamera_Drag_MainBtn(
									filterTriggerEventIndexCounter,
									cursorIndex,
									rayDir
								),
								dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_3rdPersonCamera_Drag_MainBtn
							);
						} else {
							// Unhandled camera mode.
							throw LogicException(LOC);
						}
					}
				}
			}
		}

		// Use the screenPos value and not the cursor one because the cached pos is always correct.
		dispatchWorldInteractionFilters(cursorHD, base::Touch::ButtonCode::LeftOrTouch, TouchEventType::CURSOR_DRAG, screenPos);
	}
}

void GPUEC_ScreenTouch2D::tick_cursorDrag_Alternative(
	bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
	CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& mousePosDelta, Vector2& mousePosDelta_sensitised)
{
	if (isDirty_newData || isDirty_forceUpdate) {
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Alternative, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			// Ground always dispatches, there is not filter test required.
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_ScreenCursor_Drag_Alternative(
					filterTriggerEventIndexCounter, cursorIndex,
					screenPos, mousePosDelta, mousePosDelta_sensitised
				),
				dispatcher, IWorldOrEntityInteractionFilter::Type::ScreenCursor_Drag_Alternative
			);
		}

		// Use the screenPos value and not the cursor one because the cached pos is always correct.
		dispatchWorldInteractionFilters(cursorHD, base::Touch::ButtonCode::Right, TouchEventType::CURSOR_DRAG, screenPos);
	}
}

void GPUEC_ScreenTouch2D::processAction_cursorUp_Main(int cursorIndex, Vector2& screenPos) {
	filterTriggerEventIndexCounter++;

	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::GroundUp, false);
	if(dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		// Compute interaction info.
		Vector3 groundPos{};
		
		gw->getMain()->camera2DCoordsProjectionTo3DGroundPos(
			screenPos/*screenPos*/,
			0.0f/*groundPlanePosZ*/,
			& groundPos/*oIntersectionPos*/,
			nullptr/*oRayDirection*/
		);

		// Ground always dispatches, there is not filter test required.
		dispatchInteraction_Filtered_WorldPos(
			controledEntityTarget->entity,
            rpg3D::WorldOrEntityInteractionEvent::getNew_GroundUp(filterTriggerEventIndexCounter, cursorIndex, groundPos),
            dispatcher, IWorldOrEntityInteractionFilter::Type::GroundUp
        );
	}

	dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::EntityUp, false);
	if(dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		// Compute interaction info.
		Ray& ray = gw->getMain()->getEnvironment3D()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
		std::shared_ptr<ArrayList<base::IGamePartMain::DT_Entity_Distance>> touchedEntitiesList = gw->getGPMain()->getEntitiesUnderTouch(ray, nullptr);

		// Do manual dispatch and check the filter.
		dispatchInteraction_Filtered_Entity(
			controledEntityTarget->entity, touchedEntitiesList,
			IWorldOrEntityInteractionFilter::Type::EntityUp, filterTriggerEventIndexCounter, cursorIndex,
            dispatcher
        );
	}

	dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		dispatchInteraction_NotFiltered(
            rpg3D::WorldOrEntityInteractionEvent::getNew_ScreenCursor_Up_Main(filterTriggerEventIndexCounter, cursorIndex, screenPos),
            dispatcher, IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Main
        );
	}
}

void GPUEC_ScreenTouch2D::processAction_cursorUp_Alternative(int cursorIndex, Vector2& screenPos) {
	filterTriggerEventIndexCounter++;

	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Alternative, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		dispatchInteraction_NotFiltered(
            rpg3D::WorldOrEntityInteractionEvent::getNew_ScreenCursor_Up_Alternative(filterTriggerEventIndexCounter, cursorIndex, screenPos),
            dispatcher, IWorldOrEntityInteractionFilter::Type::ScreenCursor_Up_Alternative
        );
	}
}

void GPUEC_ScreenTouch2D::onHookCustomFilter(rpg3D::IWorldInteractionFilter* filter) {
	super::onHookCustomFilter(filter);

	if ((filter->getCursorModeBitGroups() & base::Touch::CursorMode::Normal->bitGroup) == base::Touch::CursorMode::Normal->bitGroup) {
		// Check if handled btn id.
		if ((filter->getButtonIdBitGroups() & (base::Touch::ButtonCode::LeftOrTouch->bitGroup)) != filter->getButtonIdBitGroups()) {
			// Not all requested button_id-s are implemented.
			throw LogicException(LOC);
		}

		if ((filter->getButtonIdBitGroups() & base::Touch::ButtonCode::LeftOrTouch->bitGroup) == base::Touch::ButtonCode::LeftOrTouch->bitGroup) {
			// Check if handled action type.
			if (((filter->getTouchTypeBitGroups() & (TouchEventType::CURSOR_DOWN->bitGroup | TouchEventType::CURSOR_UP->bitGroup | TouchEventType::CURSOR_DRAG->bitGroup)) != filter->getTouchTypeBitGroups())) {
				// Not all requested action_id-s are implemented.
				throw LogicException(LOC);
			}

			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_DOWN->bitGroup) == TouchEventType::CURSOR_DOWN->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_ScreenCursor_Main_Down));
			}
			
			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_UP->bitGroup) == TouchEventType::CURSOR_UP->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_ScreenCursor_Main_Up));
			}

			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_DRAG->bitGroup) == TouchEventType::CURSOR_DRAG->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_ScreenCursor_Main_Drag));
			}
		}
		if ((filter->getButtonIdBitGroups() & base::Touch::ButtonCode::Right->bitGroup) == base::Touch::ButtonCode::Right->bitGroup) {
			// Check if handled action type.
			if (((filter->getTouchTypeBitGroups() & (TouchEventType::CURSOR_DOWN->bitGroup | TouchEventType::CURSOR_UP->bitGroup | TouchEventType::CURSOR_DRAG->bitGroup)) != filter->getTouchTypeBitGroups())) {
				// Not all requested action_id-s are implemented.
				throw LogicException(LOC);
			}

			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_DOWN->bitGroup) == TouchEventType::CURSOR_DOWN->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_ScreenCursor_Alt_Down));
			}

			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_UP->bitGroup) == TouchEventType::CURSOR_UP->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_ScreenCursor_Alt_Up));
			}

			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_DRAG->bitGroup) == TouchEventType::CURSOR_DRAG->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_ScreenCursor_Alt_Drag));
			}
		}
	}
}

void GPUEC_ScreenTouch2D::onControlledEntityChanged() {
	super::onControlledEntityChanged();

	// Always clear. Even if changed from non-nullptr to different non-nullptr.
	cursorHandlingDataList.clear();
}

void GPUEC_ScreenTouch2D::clearInput() {
	super::clearInput();

	cursorHandlingDataList.clear();
}

GPUEC_ScreenTouch2D::~GPUEC_ScreenTouch2D() {
	//void
}
