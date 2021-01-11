#include "GPUEC_LockedCursorMoved.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <rpg3D/gw/entity/module/userControlled/util/IWorldOrEntityInteractionFilter.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <rpg3D/gw/interaction/deviceinput/customSensor/SensorDataSensitizedLockedCursor.h>
#include <worldGame3D/gw/util/GameCameraModeBitGroups.h>
#include <graphics/camera/Camera.h>
#include <worldGame3D/gw/sim/IGPSim.h>
#include <rpg3D/gw/entity/module/userControlled/filters/IWorldInteractionFilter.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace rpg3D;

GPUEC_LockedCursorMoved::GPUEC_LockedCursorMoved(base::IGameWorld* gw)
	: super(gw)
{
    //void
}

GPUEC_LockedCursorMoved::CursorHandlingData* GPUEC_LockedCursorMoved::getCursorHandlingData(int cursorIndex) {
	while (cursorHandlingDataList.size() <= cursorIndex) {
		cursorHandlingDataList.appendDirect(std::make_shared<CursorHandlingData>(cursorHandlingDataList.size()));
	}

	std::shared_ptr<CursorHandlingData>& cursorHD = cursorHandlingDataList.getReference(cursorIndex);

	return cursorHD.get();
}

bool GPUEC_LockedCursorMoved::processCacheInput(std::shared_ptr<base::SensorEvent>& input) {
	bool ret = super::processCacheInput(input);

	if (input->sensorType == IManagedDeviceInput::CustomSensorTypes::SensitizedLockedCursor) {
		ret = true;

		rpg3D::SensorDataSensitizedLockedCursor* cSensorData = input->data->castToSubtype<rpg3D::SensorDataSensitizedLockedCursor*>();

		CursorHandlingData* cursorHD = getCursorHandlingData(cSensorData->cursorIndex);
		if (cursorHD->cacheScreenPos == nullptr) {
			cursorHD->cacheScreenPos = std::make_shared<Vector2>(cSensorData->screenPos);
		} else {
			cursorHD->cacheScreenPos->set(cSensorData->screenPos);
		}

		if (cSensorData->touchType == TouchEventType::CURSOR_DOWN) {
			if (cSensorData->buttonId == base::Touch::ButtonCode::LeftOrTouch) {
				processAction_lockedCursorDown_Main(cursorHD, cSensorData->buttonId, TouchEventType::CURSOR_DOWN, cSensorData->screenPos);

				dispatchWorldInteractionFilters(cursorHD, cSensorData->buttonId, cSensorData->touchType, cSensorData->screenPos);
			}else if (cSensorData->buttonId == base::Touch::ButtonCode::Right) {
				processAction_lockedCursorDown_Alternative(cursorHD, cSensorData->screenPos);

				dispatchWorldInteractionFilters(cursorHD, cSensorData->buttonId, cSensorData->touchType, cSensorData->screenPos);
			}else {
				throw LogicException(LOC);
			}
		}
		
		else if (cSensorData->touchType == TouchEventType::CURSOR_UP) {
			if (cSensorData->buttonId == base::Touch::ButtonCode::LeftOrTouch) {
				processAction_lockedCursorUp_Main(cSensorData->cursorIndex, cSensorData->screenPos);

				dispatchWorldInteractionFilters(cursorHD, cSensorData->buttonId, cSensorData->touchType, cSensorData->screenPos);
			} else if (cSensorData->buttonId == base::Touch::ButtonCode::Right) {
				processAction_lockedCursorUp_Alternative(cSensorData->cursorIndex, cSensorData->screenPos);

				dispatchWorldInteractionFilters(cursorHD, cSensorData->buttonId, cSensorData->touchType, cSensorData->screenPos);
			} else {
				throw LogicException(LOC);
			}
		}

		else if (cSensorData->touchType == TouchEventType::CURSOR_MOVE) {
			CursorHandlingData* cursorHD = getCursorHandlingData(cSensorData->cursorIndex);

			cursorHD->onData_move(cSensorData->screenPos, cSensorData->sensitisedDelta);
		}
		
		else if (cSensorData->touchType == TouchEventType::CURSOR_DRAG) {
			if (cSensorData->buttonId == base::Touch::ButtonCode::LeftOrTouch) {
				CursorHandlingData* cursorHD = getCursorHandlingData(cSensorData->cursorIndex);

				cursorHD->onData_drag_Main(cSensorData->screenPos, cSensorData->sensitisedDelta);
			}else if (cSensorData->buttonId == base::Touch::ButtonCode::Right) {
				CursorHandlingData* cursorHD = getCursorHandlingData(cSensorData->cursorIndex);

				cursorHD->onData_drag_Alternative(cSensorData->screenPos, cSensorData->sensitisedDelta);
			}else {
				throw LogicException(LOC);
			}
		}
		
		else {
			throw LogicException(LOC);
		}
	}

	return ret;
}

void GPUEC_LockedCursorMoved::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_localInput(deltaS, deltaMS, gameTimeS);

	for (int cursorIndex = 0; cursorIndex < cursorHandlingDataList.count(); cursorIndex++) {
		std::shared_ptr<CursorHandlingData>& cursorHD = cursorHandlingDataList.getReference(cursorIndex);

		onGameLoop_move(cursorHD.get());
		onGameLoop_drag_Main(cursorHD.get());
		onGameLoop_drag_Alternative(cursorHD.get());
	}
}

void GPUEC_LockedCursorMoved::onGameLoop_move(CursorHandlingData* cursorHD) {
	// Update on camera_change, unit_pos_change, data, force_update.
	bool isDirty_newData = false;
	bool isDirty_forceUpdate = false;
	bool isDirty_cameraUpdate = false;
	bool isDirty_unitPos = false;

	int cameraUpdateIndex = gw->getMain()->getEnvironment3D()->peekCamera()->getUpdateIndex();
	Vector3* unitPos = controledEntityTarget->entity->getPos();

	if (cursorHD->isData_move && !cursorHD->data_move_lockedCursorSensitisedDelta.equals(0.0f, 0.0f)) {
		// New data.
		isDirty_newData = true;

		cursorHD->isData_move = false;
	}
	if (isDirty_newData || (localCache_isCursorBlockedMode)) {
		// There is updating_now or was_a_initial_update and locked_cursor_mode.

		if (isDirtyControlType) {
			// Control type changed.
			isDirty_forceUpdate = true;
		}
		if (cursorHD->lastDispatched_move_cameraUpdateIndex != cameraUpdateIndex) {
			// Camera changed.
			isDirty_cameraUpdate = true;
		}
		if (cursorHD->lastDispatched_move_unitPos == nullptr || !cursorHD->lastDispatched_move_unitPos->equals(unitPos)) {
			// Unit pos changed.
			isDirty_unitPos = true;
		}
	}

	if (localCache_isCursorBlockedMode && (isDirty_newData || cursorHD->cacheScreenPos != nullptr) && (isDirty_newData || isDirty_forceUpdate || isDirty_cameraUpdate || isDirty_unitPos)) {
		Vector2* newScreenPos = isDirty_newData ? &cursorHD->data_move_screenPos : cursorHD->cacheScreenPos.get();

		Vector2 lockedCursorSensitisedDelta{};
		if (isDirty_newData) {
			lockedCursorSensitisedDelta.set(cursorHD->data_move_lockedCursorSensitisedDelta);
		} else {
			lockedCursorSensitisedDelta.set(0.0f, 0.0f);
		}

		tick_lockedCursorMove(
			isDirty_newData, isDirty_forceUpdate, isDirty_cameraUpdate, isDirty_unitPos,
			cursorHD, cursorHD->cursorIndex, *newScreenPos, lockedCursorSensitisedDelta);

		if (cursorHD->lastDispatched_move_unitPos == nullptr) {
			cursorHD->lastDispatched_move_unitPos = std::make_shared<Vector3>(unitPos);
		} else {
			cursorHD->lastDispatched_move_unitPos->set(*unitPos);
		}
		cursorHD->lastDispatched_move_cameraUpdateIndex = cameraUpdateIndex;
	}
}

void GPUEC_LockedCursorMoved::onGameLoop_drag_Main(CursorHandlingData* cursorHD) {
	// Update on camera_change, unit_pos_change, data, force_update.
	bool isDirty_newData = false;
	bool isDirty_forceUpdate = false;
	bool isDirty_cameraUpdate = false;
	bool isDirty_unitPos = false;

	int cameraUpdateIndex = gw->getMain()->getEnvironment3D()->peekCamera()->getUpdateIndex();
	Vector3* unitPos = controledEntityTarget->entity->getPos();

	if (cursorHD->isData_drag_Main && !cursorHD->data_drag_Main_lockedCursorSensitisedDelta.equals(0.0f, 0.0f)) {
		// New data.
		isDirty_newData = true;

		cursorHD->isData_drag_Main = false;
	}
	if (isDirty_newData || (localCache_isCursorBlockedMode)) {
		// There is updating_now or was_a_initial_update and locked_cursor_mode.

		if (isDirtyControlType) {
			// Control type changed.
			isDirty_forceUpdate = true;
		}
		if (cursorHD->lastDispatched_drag_Main_cameraUpdateIndex != cameraUpdateIndex) {
			// Camera changed.
			isDirty_cameraUpdate = true;
		}
		if (cursorHD->lastDispatched_drag_Main_unitPos == nullptr || !cursorHD->lastDispatched_drag_Main_unitPos->equals(unitPos)) {
			// Unit pos changed.
			isDirty_unitPos = true;
		}
	}

	if (localCache_isCursorBlockedMode && (isDirty_newData || cursorHD->cacheScreenPos != nullptr) && (isDirty_newData || isDirty_forceUpdate || isDirty_cameraUpdate || isDirty_unitPos)) {
		Vector2* newScreenPos = isDirty_newData ? &cursorHD->data_drag_Main_screenPos : cursorHD->cacheScreenPos.get();
		Vector2 mousePosDelta{};

		if (isDirty_newData) {
			mousePosDelta.set(cursorHD->data_drag_Main_lockedCursorSensitisedDelta);
		} else {
			mousePosDelta.set(0.0f, 0.0f);
		}

		tick_lockedCursorDrag_Main(
			isDirty_newData, isDirty_forceUpdate, isDirty_cameraUpdate, isDirty_unitPos,
			cursorHD, cursorHD->cursorIndex, *newScreenPos, mousePosDelta);

		if (cursorHD->lastDispatched_drag_Main_unitPos == nullptr) {
			cursorHD->lastDispatched_drag_Main_unitPos = std::make_shared<Vector3>(unitPos);
		} else {
			cursorHD->lastDispatched_drag_Main_unitPos->set(*unitPos);
		}
		cursorHD->lastDispatched_drag_Main_cameraUpdateIndex = cameraUpdateIndex;
	}
}

void GPUEC_LockedCursorMoved::onGameLoop_drag_Alternative(CursorHandlingData* cursorHD) {
	// Update on camera_change, unit_pos_change, data, force_update.
	bool isDirty_newData = false;
	bool isDirty_forceUpdate = false;
	bool isDirty_cameraUpdate = false;
	bool isDirty_unitPos = false;

	int cameraUpdateIndex = gw->getMain()->getEnvironment3D()->peekCamera()->getUpdateIndex();
	Vector3* unitPos = controledEntityTarget->entity->getPos();

	if (cursorHD->isData_drag_Alternative && !cursorHD->data_drag_Alternative_lockedCursorSensitisedDelta.equals(0.0f, 0.0f)) {
		// New data.
		isDirty_newData = true;

		cursorHD->isData_drag_Alternative = false;
	}
	if (isDirty_newData || (localCache_isCursorBlockedMode)) {
		// There is updating_now or was_a_initial_update and locked_cursor_mode.

		if (isDirtyControlType) {
			// Control type changed.
			isDirty_forceUpdate = true;
		}
		if (cursorHD->lastDispatched_drag_Alternative_cameraUpdateIndex != cameraUpdateIndex) {
			// Camera changed.
			isDirty_cameraUpdate = true;
		}
		if (cursorHD->lastDispatched_drag_Alternative_unitPos == nullptr || !cursorHD->lastDispatched_drag_Alternative_unitPos->equals(unitPos)) {
			// Unit pos changed.
			isDirty_unitPos = true;
		}
	}

	if (localCache_isCursorBlockedMode && (isDirty_newData || cursorHD->cacheScreenPos != nullptr) && (isDirty_newData || isDirty_forceUpdate || isDirty_cameraUpdate || isDirty_unitPos)) {
		Vector2* newScreenPos = isDirty_newData ? &cursorHD->data_drag_Alternative_screenPos : cursorHD->cacheScreenPos.get();

		Vector2 mousePosDelta{};
		if (isDirty_newData) {
			mousePosDelta.set(cursorHD->data_drag_Alternative_lockedCursorSensitisedDelta);
		} else {
			mousePosDelta.set(0.0f, 0.0f);
		}

		tick_lockedCursorDrag_Alternative(
			isDirty_newData, isDirty_forceUpdate, isDirty_cameraUpdate, isDirty_unitPos,
			cursorHD, cursorHD->cursorIndex, *newScreenPos, mousePosDelta);

		if (cursorHD->lastDispatched_drag_Alternative_unitPos == nullptr) {
			cursorHD->lastDispatched_drag_Alternative_unitPos = std::make_shared<Vector3>(unitPos);
		} else {
			cursorHD->lastDispatched_drag_Alternative_unitPos->set(*unitPos);
		}
		cursorHD->lastDispatched_drag_Alternative_cameraUpdateIndex = cameraUpdateIndex;
	}
}

void GPUEC_LockedCursorMoved::dispatchWorldInteractionFilters(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId, TouchEventType* touchType, Vector2& screenPos) {
	int cameraModeBitGroup = gw->getSim()->getCameraMode();
	int blockingPriorityValue = -1;

	ListDL<IWorldInteractionFilter*>* listToEnumerate = nullptr;
	if (buttonId == base::Touch::ButtonCode::LeftOrTouch) {
		if (touchType == TouchEventType::CURSOR_DOWN) {
			listToEnumerate = &customFiltersLists_LockedCursor_Main_Down;
		} else if (touchType == TouchEventType::CURSOR_UP) {
			listToEnumerate = &customFiltersLists_LockedCursor_Main_Up;
		} else if (touchType == TouchEventType::CURSOR_DRAG) {
			listToEnumerate = &customFiltersLists_LockedCursor_Main_Drag;
		}
	} else if (buttonId == base::Touch::ButtonCode::Right) {
		if (touchType == TouchEventType::CURSOR_DOWN) {
			listToEnumerate = &customFiltersLists_LockedCursor_Alt_Down;
		} else if (touchType == TouchEventType::CURSOR_UP) {
			listToEnumerate = &customFiltersLists_LockedCursor_Alt_Up;
		} else if (touchType == TouchEventType::CURSOR_DRAG) {
			listToEnumerate = &customFiltersLists_LockedCursor_Alt_Drag;
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
					base::Touch::CursorMode::Locked, buttonId, touchType,
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

void GPUEC_LockedCursorMoved::processAction_lockedCursorDown_Main(CursorHandlingData* cursorHD, base::Touch::ButtonCode* buttonId, TouchEventType* touchType, Vector2& screenPos) {
	filterTriggerEventIndexCounter++;

	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Main, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		dispatchInteraction_NotFiltered(
			rpg3D::WorldOrEntityInteractionEvent::getNew_LockedCursor_Down_Main(
				filterTriggerEventIndexCounter,
				cursorHD->cursorIndex,
				screenPos
			),
			dispatcher, IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Main
		);
	}

	processAction_lockedCursorDown_Main_B(cursorHD, screenPos);
	
	//asd_011;
	dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::EntityDown, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		// Compute interaction info.
		Ray& ray = gw->getMain()->getEnvironment3D()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
		std::shared_ptr<ArrayList<base::IGamePartMain::DT_Entity_Distance>> touchedEntitiesList = gw->getGPMain()->getEntitiesUnderTouch(ray, nullptr);

		// Do manual dispatch and check the filter.
		dispatchInteraction_Filtered_Entity(
			controledEntityTarget->entity, touchedEntitiesList,
			IWorldOrEntityInteractionFilter::Type::EntityDown, filterTriggerEventIndexCounter, cursorHD->cursorIndex,
			dispatcher
		);
	}
}

void GPUEC_LockedCursorMoved::processAction_lockedCursorDown_Main_B(CursorHandlingData* cursorHD, Vector2& screenPos) {
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
	
	bool isDispatched_UnitFeetGroundPlanePos_Down_MainBtn = false;
	bool isRaySet = false;
	Vector3 rayDir{};

	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		// Compute interaction info.
		Vector3 unitFeetGroundPlanePos{};
		bool isIntersection = gw->getGPMain()->camera2DCoordsProjectionTo3DGroundPos(
			screenPos,
			controledEntityTarget->entity->getZ(),
			&unitFeetGroundPlanePos,
			&rayDir
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
			} else if(camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
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
						screenPos,
						&rayDir
					);
				}

				// Ground always dispatches, there is not filter test required.
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

void GPUEC_LockedCursorMoved::processAction_lockedCursorDown_Alternative(CursorHandlingData* cursorHD, Vector2& screenPos) {
	filterTriggerEventIndexCounter++;
	
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Alternative, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		dispatchInteraction_NotFiltered(
			rpg3D::WorldOrEntityInteractionEvent::getNew_LockedCursor_Down_Alternative(
				filterTriggerEventIndexCounter,
				cursorHD->cursorIndex,
				screenPos
			),
			dispatcher, IWorldOrEntityInteractionFilter::Type::LockedCursor_Down_Alternative
		);
	}
}

void GPUEC_LockedCursorMoved::tick_lockedCursorMove(
	bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
	CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& lockedCursorSensitisedDelta)
{
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher;
	if (isDirty_forceUpdate || isDirty_newData) {
		dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::LockedCursor_Move, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_LockedCursor_Move(
					-1,
					cursorIndex,
					screenPos, lockedCursorSensitisedDelta
				),
				dispatcher, IWorldOrEntityInteractionFilter::Type::LockedCursor_Move
			);
		}
	}

	if (isDirty_newData) {
		dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3D_Delta, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_ContinuousFocusView3D_Delta(-1, lockedCursorSensitisedDelta),
				dispatcher, IWorldOrEntityInteractionFilter::Type::ContinuousFocusView3D_Delta
			);
		}
	}
}

void GPUEC_LockedCursorMoved::tick_lockedCursorDrag_Main(
	bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
	CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& lockedCursorSensitisedDelta)
{
	if (isDirty_newData || isDirty_forceUpdate) {
		// Drag filter main.
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::LockedCursor_Drag_Main, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_LockedCursor_Drag_Main(-1, screenPos, lockedCursorSensitisedDelta),
				dispatcher, IWorldOrEntityInteractionFilter::Type::LockedCursor_Drag_Main
			);
		}
	}

	tick_lockedCursorDrag_Main_B(
		isDirty_newData, isDirty_forceUpdate, isDirty_cameraUpdate, isDirty_unitPos,
		cursorHD, cursorIndex, screenPos, lockedCursorSensitisedDelta);
}

void GPUEC_LockedCursorMoved::tick_lockedCursorDrag_Main_B(
	bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
	CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& lockedCursorSensitisedDelta)
{
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
				bool isIntersection = gw->getGPMain()->camera2DCoordsProjectionTo3DGroundPos(
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
								cursorHD->cursorIndex,
								unitFeetGroundPlanePos
							),
							dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetGroundPlanePos_1stPersonCamera_Drag_MainBtn
						);
					} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
						dispatchInteraction_Filtered_WorldPos(
							controledEntityTarget->entity,
							rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetGroundPlanePos_3rdPersonCamera_Drag_MainBtn(
								filterTriggerEventIndexCounter,
								cursorHD->cursorIndex,
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
									cursorHD->cursorIndex,
									rayDir
								),
								dispatcher, IWorldOrEntityInteractionFilter::Type::UnitFeetHalfSphereDomeDir_1stPersonCamera_Drag_MainBtn
							);
						} else if (camMode == GameCameraModeBitGroups::followerThirdPersonFixedGlobalRotation) {
							dispatchInteraction_NotFiltered(
								rpg3D::WorldOrEntityInteractionEvent::getNew_UnitFeetHalfSphereDomeDir_3rdPersonCamera_Drag_MainBtn(
									filterTriggerEventIndexCounter,
									cursorHD->cursorIndex,
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

void GPUEC_LockedCursorMoved::tick_lockedCursorDrag_Alternative(
	bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraUpdate, bool isDirty_unitPos,
	CursorHandlingData* cursorHD, int cursorIndex, Vector2& screenPos, Vector2& lockedCursorSensitisedDelta)
{
	if (isDirty_newData || isDirty_forceUpdate) {
		// Drag filter alt.
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::LockedCursor_Drag_Alternative, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_LockedCursor_Drag_Alternative(
					-1,
					screenPos, lockedCursorSensitisedDelta
				),
				dispatcher, IWorldOrEntityInteractionFilter::Type::LockedCursor_Drag_Alternative
			);
		}

		// Use the screenPos value and not the cursor one because the cached pos is always correct.
		dispatchWorldInteractionFilters(cursorHD, base::Touch::ButtonCode::Right, TouchEventType::CURSOR_DRAG, screenPos);
	}
}

void GPUEC_LockedCursorMoved::processAction_lockedCursorUp_Main(int cursorIndex, Vector2& screenPos) {
	filterTriggerEventIndexCounter++;
	
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		dispatchInteraction_NotFiltered(
			rpg3D::WorldOrEntityInteractionEvent::getNew_LockedCursor_Up_Main(
				filterTriggerEventIndexCounter,
				cursorIndex,
				screenPos
			),
			dispatcher, IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Main
		);
	}
}

void GPUEC_LockedCursorMoved::processAction_lockedCursorUp_Alternative(int cursorIndex, Vector2& screenPos) {
	filterTriggerEventIndexCounter++;
	
	base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Alternative, false);
	if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
		dispatchInteraction_NotFiltered(
			rpg3D::WorldOrEntityInteractionEvent::getNew_LockedCursor_Up_Alternative(
				filterTriggerEventIndexCounter,
				cursorIndex,
				screenPos
			),
			dispatcher, IWorldOrEntityInteractionFilter::Type::LockedCursor_Up_Alternative
		);
	}
}

void GPUEC_LockedCursorMoved::onHookCustomFilter(rpg3D::IWorldInteractionFilter* filter) {
	super::onHookCustomFilter(filter);

	if ((filter->getCursorModeBitGroups() & base::Touch::CursorMode::Locked->bitGroup) == base::Touch::CursorMode::Locked->bitGroup) {
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
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_LockedCursor_Main_Down));
			}
			
			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_UP->bitGroup) == TouchEventType::CURSOR_UP->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_LockedCursor_Main_Up));
			}
			
			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_DRAG->bitGroup) == TouchEventType::CURSOR_DRAG->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_LockedCursor_Main_Drag));
			}
		}
		if ((filter->getButtonIdBitGroups() & base::Touch::ButtonCode::Right->bitGroup) == base::Touch::ButtonCode::Right->bitGroup) {
			// Check if handled action type.
			if (((filter->getTouchTypeBitGroups() & (TouchEventType::CURSOR_DOWN->bitGroup | TouchEventType::CURSOR_UP->bitGroup | TouchEventType::CURSOR_DRAG->bitGroup)) != filter->getTouchTypeBitGroups())) {
				// Not all requested action_id-s are implemented.
				throw LogicException(LOC);
			}

			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_DOWN->bitGroup) == TouchEventType::CURSOR_DOWN->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_LockedCursor_Alt_Down));
			}

			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_UP->bitGroup) == TouchEventType::CURSOR_UP->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_LockedCursor_Alt_Up));
			}

			if ((filter->getTouchTypeBitGroups() & TouchEventType::CURSOR_DRAG->bitGroup) == TouchEventType::CURSOR_DRAG->bitGroup) {
				filter->getHookedListEntries().appendDirect(InsertFilterInList(filter, customFiltersLists_LockedCursor_Alt_Drag));
			}
		}
	}
}

GPUEC_LockedCursorMoved::~GPUEC_LockedCursorMoved() {
    //void
}
