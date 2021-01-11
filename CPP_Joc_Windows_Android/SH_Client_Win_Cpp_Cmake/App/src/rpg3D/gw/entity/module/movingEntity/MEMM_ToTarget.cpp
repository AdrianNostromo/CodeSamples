#include "MEMM_ToTarget.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <worldGame3D/gw/entity/module/lookAt/LookAtModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/math/Math.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <worldGame3D/gw/entity/util/IPositional3DHandler.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigDirect.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsPush.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsRollingSphere.h>
#include <worldGame3D/gw/entity/util/WrappedWorldEntity.h>

using namespace rpg3D;

int MEMM_ToTarget::ActionToTargetMovementContinuous::TYPE = super::NewTypeIndex();

const int MEMM_ToTarget::MOVE_REQUEST_TYPE_ToTarget = newMoveRequestType();

MEMM_ToTarget::MEMM_ToTarget(
	IWorldEntity* entity,
	rpg3D::MovingEntityTemplate* t
)
	: super(entity, t)
{
	//void
}

void MEMM_ToTarget::createBMain() {
	super::createBMain();

	actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);

	lookAt = entity->getComponentAs<ILookAtModule*>(true/*mustExist*/);

	physics3DModule = entity->getComponentAs<rpg3D::IPhysics3DModule*>(false/*mustExist*/);

	actions->setActionProcessor(
		ActionToTargetMovementContinuous::TYPE,
		std::bind(&MEMM_ToTarget::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

std::shared_ptr<worldGame3D::EntityAction> MEMM_ToTarget::buildAction_ActionToTargetMovementContinuous(
	int controlType,
	Vector2& targetPosOrOffset, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
	float maxRange, bool isStopAtDestination,
	bool isAutoTurn, bool isInstantTurn,
	std::string& initiatorIdentifier,
	int movementTypeOverride)
{
	std::shared_ptr<ActionToTargetMovementContinuous> action = std::make_shared<ActionToTargetMovementContinuous>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		controlType,
		targetPosOrOffset, targetEntity,
		maxRange, isStopAtDestination,
		isAutoTurn, isInstantTurn,
		initiatorIdentifier,
		movementTypeOverride
	);

	return action;
}

void MEMM_ToTarget::addAction_ActionToTargetMovementContinuous(
	int controlType,
	Vector2& targetPosOrOffset, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
	float maxRange, bool isStopAtDestination,
	bool isAutoTurn, bool isInstantTurn,
	std::string& initiatorIdentifier,
	int movementTypeOverride)
{
	actions->addAction(buildAction_ActionToTargetMovementContinuous(
		controlType,
		targetPosOrOffset, targetEntity,
		maxRange, isStopAtDestination,
		isAutoTurn, isInstantTurn,
		initiatorIdentifier,
		movementTypeOverride
	));
}

void MEMM_ToTarget::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if(action.getType() == ActionToTargetMovementContinuous::TYPE) {
		onEntityAction_ActionToTargetMovementContinuous((ActionToTargetMovementContinuous*)action.getRawSubclassPointer(ActionToTargetMovementContinuous::TYPE));
	}else {
		throw LogicException(LOC);
	}
}

void MEMM_ToTarget::onEntityAction_ActionToTargetMovementContinuous(ActionToTargetMovementContinuous* action) {
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

	if (action->ticksCounter == 0) {
		e->dispatchEvent(*EVENT_move_Start->setData_initiatorIdentifier(action->initiatorIdentifier));
	}

	if (action->targetEntity != nullptr && action->targetEntity->entity == nullptr) {
		// The target entity was removed, stop the movement.
		action->persistentActionFinished();

		return;
	}

	float currentX = entity->getX();
	float currentY = entity->getY();

	float destPosX = action->targetPosOrOffset.x;
	float destPosY = action->targetPosOrOffset.y;
	if (action->targetEntity != nullptr) {
		destPosX += action->targetEntity->entity->getX();
		destPosY += action->targetEntity->entity->getY();
	}

	if (action->controlType == ControlType::LocalEntityDirection_BackForwardOnly) {
		//void
	} else {
		// Currently only LocalEntityDirection_BackForwardOnly is implemented, implement new types when needed.
		throw LogicException(LOC);
	}

	float destAngZRad;
	Vector2 destDirVector{};
	if (currentX == destPosX && currentY == destPosY) {
		destAngZRad = Math::toRadians(entity->getRot()->getAngleAround(Vector3::Z));
	} else {
		destAngZRad = Math::atan2(destPosY - currentY, destPosX - currentX);
	}
	destDirVector.x = Math::cosRad(destAngZRad);
	destDirVector.y = Math::sinRad(destAngZRad);

	float deltaDist = Math::dist(
		currentX, currentY,
		destPosX, destPosY
	);

	if (action->isAutoTurn) {
		float newAngDegZ = Math::toDegrees(destAngZRad);
		entity->getPositional3D()->setCardinalRotation(newAngDegZ);
		lookAt->setLookAtAnglesDeg(
			newAngDegZ,
			0.0f
		);
	}

	rpg3D::MovementConfig* movementConfig;
	if (action->movementTypeOverride >= 0) {
		movementConfig = t->getMovementConfigMustExist(action->movementTypeOverride);
	} else {
		movementConfig = t->getDefaultMovementConfigMustExist();
	}
	if (movementConfig->getType() == rpg3D::MovementConfigPhysicsPush::TYPE) {
		rpg3D::MovementConfigPhysicsPush* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigPhysicsPush*>(rpg3D::MovementConfigPhysicsPush::TYPE);

		if (action->isStopAtDestination && deltaDist <= action->maxRange) {
			physics3DModule->setLinearForce(
				0.0f, 0.0f, 0.0f,
				true/*isContinuousUpdate*/, &cMovementConfig->moveDamping,
				cMovementConfig->applyForceToMovementPBodyOnly,
				true/*useCentralForceInsteadOfVelocitySet*/
			);

			action->persistentActionFinished();
			
			e->dispatchEvent(*EVENT_move_destinationReached->setData_initiatorIdentifier(action->initiatorIdentifier));
		} else {
			physics3DModule->setLinearForce(
				destDirVector.x * cMovementConfig->moveForce, destDirVector.y * cMovementConfig->moveForce, 0.0f * cMovementConfig->moveForce,
				true/*isContinuousUpdate*/, &cMovementConfig->moveDamping,
				cMovementConfig->applyForceToMovementPBodyOnly,
				true/*useCentralForceInsteadOfVelocitySet*/
			);
		}
	} else if (movementConfig->getType() == rpg3D::MovementConfigPhysicsRollingSphere::TYPE) {
		rpg3D::MovementConfigPhysicsRollingSphere* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigPhysicsRollingSphere*>(rpg3D::MovementConfigPhysicsRollingSphere::TYPE);

		if (action->isStopAtDestination && deltaDist <= action->maxRange) {
			physics3DModule->setAngularVelocity(
				0.0f, 0.0f, 0.0f,
				true/*isContinuousUpdate*/, &cMovementConfig->moveAngularDamping
			);

			action->persistentActionFinished();
			
			e->dispatchEvent(*EVENT_move_destinationReached->setData_initiatorIdentifier(action->initiatorIdentifier));
		} else {
			physics3DModule->setAngularVelocity(
				-destDirVector.y * cMovementConfig->moveAngularVelocity, destDirVector.x * cMovementConfig->moveAngularVelocity, 0.0f * cMovementConfig->moveAngularVelocity,
				true/*isContinuousUpdate*/, &cMovementConfig->moveAngularDamping
			);
		}
	} else if (movementConfig->getType() == rpg3D::MovementConfigDirect::TYPE) {
		rpg3D::MovementConfigDirect* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigDirect*>(rpg3D::MovementConfigDirect::TYPE);

		float tickMoveAmount = cMovementConfig->moveSpeedS * action->deltaS;

		float newX = currentX;
		float newY = currentY;

		if (tickMoveAmount >= deltaDist - action->maxRange) {
			// Because of the maxRange, compute the movement amount needed to get in range.
			tickMoveAmount = Math::max(0.0f, deltaDist - action->maxRange);

			if (action->isStopAtDestination) {
				action->persistentActionFinished();
				
				e->dispatchEvent(*EVENT_move_destinationReached->setData_initiatorIdentifier(action->initiatorIdentifier));
			}
		}

		if (tickMoveAmount != 0.0f) {
			newX += destDirVector.x * tickMoveAmount;
			newY += destDirVector.y * tickMoveAmount;
		}

		entity->setPos(
			newX,
			newY,
			entity->getZ()
		);
	} else {
		throw LogicException(LOC);
	}
}

MEMM_ToTarget::~MEMM_ToTarget() {
	//void
}
