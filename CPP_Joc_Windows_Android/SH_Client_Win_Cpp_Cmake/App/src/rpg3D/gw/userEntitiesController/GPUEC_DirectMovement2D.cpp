#include "GPUEC_DirectMovement2D.h"
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/interaction/deviceinput/IManagedDeviceInput.h>
#include <rpg3D/gw/entity/module/userControlled/util/IWorldOrEntityInteractionFilter.h>
#include <rpg3D/gw/entity/module/userControlled/IUserControlledModule.h>
#include <rpg3D/gw/interaction/deviceinput/customSensor/SensorDataDirectionalMovement.h>
#include <graphics/camera/Camera.h>

using namespace rpg3D;

GPUEC_DirectMovement2D::GPUEC_DirectMovement2D(base::IGameWorld* gw)
	: super(gw)
{
    //void
}

bool GPUEC_DirectMovement2D::processCacheInput(std::shared_ptr<base::SensorEvent>& input) {
	bool ret = super::processCacheInput(input);

	if (input->sensorType == IManagedDeviceInput::CustomSensorTypes::DirectionalMovement) {
		ret = true;

		rpg3D::SensorDataDirectionalMovement* cSensorData = input->data->castToSubtype<rpg3D::SensorDataDirectionalMovement*>();

		data_moveDirInput.set(cSensorData->moveDirInput);
		isData_moveDirInput = true;
	}

	return ret;
}

void GPUEC_DirectMovement2D::onGameLoop_localInput(float deltaS, int deltaMS, float gameTimeS) {
	super::onGameLoop_localInput(deltaS, deltaMS, gameTimeS);

	bool isDirty_newData = false;
	bool isDirty_forceUpdate = false;
	bool isDirty_cameraDir = false;
	const Vector3* cameraDir = gw->getMain()->getEnvironment3D()->peekCamera()->getDirection();

	if (isData_moveDirInput) {
		// New data exists.
		isData_moveDirInput = false;

		isDirty_newData = true;
	}
	if (isDirty_newData || lastDispatch_moveInput != nullptr/*Directional data exists.*/) {
		// There was a initial update or updating now.
		if (isDirtyControlType) {
			// Control type changed.
			isDirty_forceUpdate = true;
		}
		
		if (lastDispatch_cameraDir == nullptr || !lastDispatch_cameraDir->equals(gw->getMain()->getEnvironment3D()->peekCamera()->getDirection())) {
			// Camera_dir_changed.

			isDirty_cameraDir = true;
		}
	}

	if (isDirty_newData || isDirty_forceUpdate || isDirty_cameraDir) {
		tick_cameraDirectionMovement(isDirty_newData, isDirty_forceUpdate, isDirty_cameraDir, data_moveDirInput, *cameraDir);
		tick_strafeMovement(isDirty_newData, isDirty_forceUpdate, isDirty_cameraDir, data_moveDirInput, *cameraDir);
		
		if (lastDispatch_moveInput == nullptr) {
			lastDispatch_moveInput = std::make_shared<Vector2>(data_moveDirInput);
		}else {
			lastDispatch_moveInput->set(data_moveDirInput);
		}
		if (lastDispatch_cameraDir == nullptr) {
			lastDispatch_cameraDir = std::make_shared<Vector3>(cameraDir);
		}else {
			lastDispatch_cameraDir->set(cameraDir);
		}
	}
}

void GPUEC_DirectMovement2D::tick_cameraDirectionMovement(bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraDir, Vector2& moveDirInput, const Vector3& cameraDir) {
	if (isDirty_newData || isDirty_forceUpdate || isDirty_cameraDir) {
		// Pool the movement keys status and the camera direction to check for changes.
		Vector3 camDir{ cameraDir };
		camDir.z = 0.0f;
		camDir.nor();

		// Grab the back, front direction as the camera direction.
		// Set the z component to 0.0f.
		// Normalise the direction.
		Vector3 dirBackFront{ camDir };
		dirBackFront.z = 0.0f;
		dirBackFront.nor();

		Vector3 dirLeftRight{ dirBackFront };
		dirLeftRight.crs(Vector3::Z);
		dirLeftRight.z = 0.0f;
		dirLeftRight.nor();

		Vector3 cameraSpaceUnitMoveDir{};
		cameraSpaceUnitMoveDir.add(
			dirBackFront.x * moveDirInput.y,
			dirBackFront.y * moveDirInput.y,
			0.0f
		);
		cameraSpaceUnitMoveDir.add(
			dirLeftRight.x * moveDirInput.x,
			dirLeftRight.y * moveDirInput.x,
			0.0f
		);
		cameraSpaceUnitMoveDir.z = 0.0f;
		cameraSpaceUnitMoveDir.clipLength(1.0f);

		// If the final move direction is not the same as the last update, call setDirectMovementDirection(...).
		if (isDirtyControlType || lastDispatched_worldSpaceMoveDirVector.x != cameraSpaceUnitMoveDir.x || lastDispatched_worldSpaceMoveDirVector.y != cameraSpaceUnitMoveDir.y) {
			lastDispatched_worldSpaceMoveDirVector.x = cameraSpaceUnitMoveDir.x;
			lastDispatched_worldSpaceMoveDirVector.y = cameraSpaceUnitMoveDir.y;

			base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::GlobalMoveDirection2D, false);
			if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
				dispatchInteraction_NotFiltered(
					rpg3D::WorldOrEntityInteractionEvent::getNew_GlobalMoveDirection2D(-1, lastDispatched_worldSpaceMoveDirVector),
					dispatcher, IWorldOrEntityInteractionFilter::Type::GlobalMoveDirection2D
				);
			}
		}
	}
}

void GPUEC_DirectMovement2D::tick_strafeMovement(bool isDirty_newData, bool isDirty_forceUpdate, bool isDirty_cameraDir, Vector2& moveDirInput, const Vector3& cameraDir) {
	if (isDirty_forceUpdate || (isDirty_newData && lastDispatched_strafeMovement_strafeLeftRight != moveDirInput.x)) {
		lastDispatched_strafeMovement_strafeLeftRight = moveDirInput.x;

		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::MovementDPadLeftRight, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_MovementDPadLeftRight(-1, lastDispatched_strafeMovement_strafeLeftRight),
				dispatcher, IWorldOrEntityInteractionFilter::Type::MovementDPadLeftRight
			);
		}
	}
	if (isDirty_forceUpdate || (isDirty_newData && lastDispatched_strafeMovement_backFront != moveDirInput.y)) {
		lastDispatched_strafeMovement_backFront = moveDirInput.y;

		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::MovementDPadBackFront, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_MovementDPadBackFront(-1, lastDispatched_strafeMovement_backFront),
				dispatcher, IWorldOrEntityInteractionFilter::Type::MovementDPadBackFront
			);
		}
	}

	if (isDirty_forceUpdate || isDirty_newData) {
		base::EventDispatcher<std::shared_ptr<rpg3D::WorldOrEntityInteractionEvent>>* dispatcher = controledEntityTarget->userControlledModule->getInteractionsDispatcher()->getDispatcher(IWorldOrEntityInteractionFilter::Type::MovementDPad, false);
		if (dispatcher != nullptr && dispatcher->getListenersCount() > 0) {
			dispatchInteraction_NotFiltered(
				rpg3D::WorldOrEntityInteractionEvent::getNew_MovementDPad(-1, lastDispatched_strafeMovement_strafeLeftRight, lastDispatched_strafeMovement_backFront),
				dispatcher, IWorldOrEntityInteractionFilter::Type::MovementDPad
			);
		}
	}
}

void GPUEC_DirectMovement2D::clearInput() {
	super::clearInput();

	lastDispatch_moveInput = nullptr;
	lastDispatch_cameraDir = nullptr;

	lastDispatched_worldSpaceMoveDirVector.idt();
	lastDispatched_strafeMovement_strafeLeftRight = 0.0f;
	lastDispatched_strafeMovement_backFront = 0.0f;
}

GPUEC_DirectMovement2D::~GPUEC_DirectMovement2D() {
	//void
}
