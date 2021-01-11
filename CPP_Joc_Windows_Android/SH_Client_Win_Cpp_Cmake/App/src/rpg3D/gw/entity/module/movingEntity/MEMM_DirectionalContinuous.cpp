#include "MEMM_DirectionalContinuous.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <worldGame3D/gw/entity/module/lookAt/LookAtModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/math/Math.h>
#include <base/ease/Ease.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <base/log/GlobalAccessLogHandler.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigDirect.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsPush.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsRollingSphere.h>

using namespace rpg3D;

int MEMM_DirectionalContinuous::ActionSetDirectionalMovementContinuous::TYPE = super::NewTypeIndex();

const int MEMM_DirectionalContinuous::MOVE_REQUEST_TYPE_Directional = newMoveRequestType();

MEMM_DirectionalContinuous::MEMM_DirectionalContinuous(
	IWorldEntity* entity,
	rpg3D::MovingEntityTemplate* t)
	: super(entity, t)
{
	//void
}

void MEMM_DirectionalContinuous::createBMain() {
	super::createBMain();

	actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);

	lookAt = entity->getComponentAs<ILookAtModule*>(true/*mustExist*/);

	physics3DModule = entity->getComponentAs<rpg3D::IPhysics3DModule*>(false/*mustExist*/);

	actions->setActionProcessor(
		ActionSetDirectionalMovementContinuous::TYPE,
		std::bind(&MEMM_DirectionalContinuous::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void MEMM_DirectionalContinuous::addAction_ActionSetDirectionalMovementContinuous(
	int controlType,
	Vector2& dir,
	bool isAutoTurn, bool isInstantTurn)
{
	Vector2 v{dir};
	v.clipLength(1.0f);// Make sure the length is <=1.0f;
	
	actions->addAction(std::make_shared<ActionSetDirectionalMovementContinuous>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		controlType,
		v,
		isAutoTurn, isInstantTurn
	));
}

void MEMM_DirectionalContinuous::addAction_ActionDirectionalMovementBackFrontContinuous(
	int controlType,
	float backFrontMoveVector)
{
	Vector2 v{0.0f, backFrontMoveVector};

    addAction_ActionSetDirectionalMovementContinuous(
        controlType,
        v,
        false, false
    );
}

void MEMM_DirectionalContinuous::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if(action.getType() == ActionSetDirectionalMovementContinuous::TYPE) {
		onEntityAction_ActionSetDirectionalMovementContinuous((ActionSetDirectionalMovementContinuous*)action.getRawSubclassPointer(ActionSetDirectionalMovementContinuous::TYPE));
	}else {
		throw LogicException(LOC);
	}
}

void MEMM_DirectionalContinuous::onEntityAction_ActionSetDirectionalMovementContinuous(ActionSetDirectionalMovementContinuous* action) {
	if (action->getIsReplaced() || action->getIsFinished() || !action->getIsPreRequirementsMet()) {
		// This is the last time this action is called.
		// Do action finish,cleanup logic (eg. remove active status effects from the list).

		//void
	}
	if (action->getIsReplaced() || action->getIsOverridden() || !action->getIsPreRequirementsMet()) {
		// Action replaced. This is the last time this action is called.
		// Or action is overridden. Should not do any active logic now excet for the above one in certain cases (eg. to remove active status effects from the list).

		return;
	}

	if (action->dir.x == 0.0f && action->dir.y == 0.0f) {
		rpg3D::MovementConfig* movementConfig = t->getDefaultMovementConfigMustExist();
		if (movementConfig->getType() == rpg3D::MovementConfigPhysicsPush::TYPE) {
			rpg3D::MovementConfigPhysicsPush* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigPhysicsPush*>(rpg3D::MovementConfigPhysicsPush::TYPE);

			physics3DModule->setLinearForce(
				0.0f, 0.0f, 0.0f, 
				false/*isContinuousUpdate*/, &cMovementConfig->stopDamping,
				cMovementConfig->applyForceToMovementPBodyOnly,
				true/*useCentralForceInsteadOfVelocitySet*/
			);
		} else if (movementConfig->getType() == rpg3D::MovementConfigPhysicsRollingSphere::TYPE) {
			rpg3D::MovementConfigPhysicsRollingSphere* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigPhysicsRollingSphere*>(rpg3D::MovementConfigPhysicsRollingSphere::TYPE);

			physics3DModule->setAngularVelocity(
				0.0f, 0.0f, 0.0f,
				false/*isContinuousUpdate*/, &cMovementConfig->stopAngularDamping
			);
		} else if (movementConfig->getType() == rpg3D::MovementConfigDirect::TYPE) {
			//void
		} else {
			throw LogicException(LOC);
		}

		if (action->ticksCounter == 0) {
			e->dispatchEvent(*EVENT_move_Stop);
		}

		action->persistentActionFinished();
	} else {
		if (action->ticksCounter == 0) {
			e->dispatchEvent(*EVENT_move_Start);
		}

		Vector2 globalMoveDir{};
		if (action->controlType == ControlType::Global_Dir) {
			globalMoveDir.set(action->dir);
		} else if (action->controlType == ControlType::LocalEntityDirection_Strafe) {
			ConvertStrafeMovementToGlobalMovement(action->dir, e->getRot(), &globalMoveDir);
		} else {
			// Implement new types when needed.
			throw LogicException(LOC);
		}

		rpg3D::MovementConfig* movementConfig = t->getDefaultMovementConfigMustExist();
		if (movementConfig->getType() == rpg3D::MovementConfigPhysicsPush::TYPE) {
			rpg3D::MovementConfigPhysicsPush* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigPhysicsPush*>(rpg3D::MovementConfigPhysicsPush::TYPE);

			if (action->ticksCounter == 0) {
				//void
			}

			physics3DModule->setLinearForce(
				globalMoveDir.x * cMovementConfig->moveForce, globalMoveDir.y * cMovementConfig->moveForce, 0.0f * cMovementConfig->moveForce,
				true/*isContinuousUpdate*/, &cMovementConfig->moveDamping,
				cMovementConfig->applyForceToMovementPBodyOnly,
				true/*useCentralForceInsteadOfVelocitySet*/
			);
		} else if (movementConfig->getType() == rpg3D::MovementConfigPhysicsRollingSphere::TYPE) {
			rpg3D::MovementConfigPhysicsRollingSphere* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigPhysicsRollingSphere*>(rpg3D::MovementConfigPhysicsRollingSphere::TYPE);

			if (action->ticksCounter == 0) {
				//void
			}

			physics3DModule->setAngularVelocity(
				-globalMoveDir.y * cMovementConfig->moveAngularVelocity, globalMoveDir.x * cMovementConfig->moveAngularVelocity, 0.0f * cMovementConfig->moveAngularVelocity,
				true/*isContinuousUpdate*/, &cMovementConfig->moveAngularDamping
			);
		} else if (movementConfig->getType() == rpg3D::MovementConfigDirect::TYPE) {
			rpg3D::MovementConfigDirect* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigDirect*>(rpg3D::MovementConfigDirect::TYPE);

			if (action->ticksCounter == 0) {
				if (cMovementConfig->moveSpeedS < 0.0f) {
					// This entity is not confgured for manual movements. Use physics movement.
					throw LogicException(LOC);
				}
			}

			float currentX = entity->getX();
			float currentY = entity->getY();

			float tickMoveAmount = cMovementConfig->moveSpeedS * action->deltaS;

			globalMoveDir.clipLength(1.0f);

			float newX = currentX + globalMoveDir.x * tickMoveAmount;
			float newY = currentY + globalMoveDir.y * tickMoveAmount;

			entity->setPos(
				newX,
				newY,
				entity->getZ()
			);
		} else {
			throw LogicException(LOC);
		}
		if (action->isAutoTurn) {
			float entityRotZDeg = entity->getRot()->getAngleAround(Vector3::Z);

			float targetRotZRad = Math::atan2(globalMoveDir.y, globalMoveDir.x);
			float targetRotZDeg = Math::toDegrees(targetRotZRad);

			if (targetRotZDeg != entityRotZDeg) {
				float newAngDegZ;
				if (!action->isInstantTurn) {
					// Get current, target angles. Compute delta angle deg.
					float deltaRotZDeg = Math::closestAngleDeg(entityRotZDeg, targetRotZDeg);
					float deltaRotZDeg_abs = Math::abs(deltaRotZDeg);
					float deltaRotZDeg_sign = Math::sign(deltaRotZDeg);

					// Get the amount of turn per tick.
					// There is a template val for max and min turn speed and interpolate between them using a sine or pow ease animation.
					float n = Math::max(0.0f, Math::min(1.0f, deltaRotZDeg_abs / 180.0f));
					n = Ease::computeEaseValue(Ease::easeOutExpo, n);
					float tickMaxRotDeg = (t->turnZSpeedDegS_min + (t->turnZSpeedDegS_max - t->turnZSpeedDegS_min) * n) * action->deltaS;

					// Get the tick turn amount.
					if (tickMaxRotDeg < deltaRotZDeg_abs) {
						// Apply the tick turn.
						newAngDegZ = entityRotZDeg + tickMaxRotDeg * deltaRotZDeg_sign;
					} else {
						newAngDegZ = targetRotZDeg;
					}
				} else {
					newAngDegZ = targetRotZDeg;
				}

				entity->setRot(Vector3::Z, newAngDegZ);
				lookAt->setLookAtAnglesDeg(
					newAngDegZ,
					0.0f
				);
			}
		}
	}
}

void MEMM_DirectionalContinuous::ConvertStrafeMovementToGlobalMovement(Vector2& moveDir, Quaternion* entityRot, Vector2* oGlobalMoveDir) {
	Vector3 unitDir{Vector3::X};

	unitDir.mul(*entityRot);
	unitDir.z = 0.0f;
	unitDir.nor();

	// Grab the back, front direction as the unit direction.
	// Set the z component to 0.0f.
	// Normalise the direction.
	Vector3 dirBackFront{unitDir};
	dirBackFront.z = 0.0f;
	dirBackFront.nor();

	Vector3 dirLeftRight{dirBackFront};
	dirLeftRight.crs(Vector3::Z);
	dirLeftRight.z = 0.0f;
	dirLeftRight.nor();

	Vector3 worldSpaceUnitMoveDir{};
	worldSpaceUnitMoveDir.add(
		dirBackFront.x * moveDir.y,
		dirBackFront.y * moveDir.y,
		0.0f
	);
	worldSpaceUnitMoveDir.add(
		dirLeftRight.x * moveDir.x,
		dirLeftRight.y * moveDir.x,
		0.0f
	);
	worldSpaceUnitMoveDir.z = 0.0f;
	worldSpaceUnitMoveDir.clipLength(1.0f);

	oGlobalMoveDir->set(worldSpaceUnitMoveDir.x, worldSpaceUnitMoveDir.y);
}

MEMM_DirectionalContinuous::~MEMM_DirectionalContinuous() {
	//void
}
