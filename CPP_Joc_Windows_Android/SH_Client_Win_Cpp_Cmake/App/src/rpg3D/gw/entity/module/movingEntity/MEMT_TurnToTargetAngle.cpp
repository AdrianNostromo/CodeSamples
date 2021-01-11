#include "MEMT_TurnToTargetAngle.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <worldGame3D/gw/entity/module/lookAt/LookAtModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/math/Math.h>
#include <worldGame3D/gw/entity/util/IPositional3DHandler.h>

using namespace rpg3D;

int MEMT_TurnToTargetAngle::ActionToTargetTurning::TYPE = super::NewTypeIndex();

const int MEMT_TurnToTargetAngle::TURN_REQUEST_TYPE_ToTarget = newTurnRequestType();

MEMT_TurnToTargetAngle::MEMT_TurnToTargetAngle(
	IWorldEntity* entity,
	rpg3D::MovingEntityTemplate* t
)
	: super(entity, t)
{
	//void
}

void MEMT_TurnToTargetAngle::createBMain() {
	super::createBMain();

	actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);

	lookAt = entity->getComponentAs<ILookAtModule*>(true/*mustExist*/);

	actions->setActionProcessor(
		ActionToTargetTurning::TYPE,
		std::bind(&MEMT_TurnToTargetAngle::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

std::shared_ptr<worldGame3D::EntityAction> MEMT_TurnToTargetAngle::buildAction_ActionToTargetTurning(
	int persistenceDurationMS,
	int priority,
	int extraGroupsBitMap,
	int extraPreRequirementsGroupsBitMask,
	int extraReplaceGroupsBitMask,
	int extraOverridesGroupsBitMask,
	float targetRotZDeg, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
	bool isStopAtDestination,
	bool isInstantTurn)
{
	std::shared_ptr<ActionToTargetTurning> action = std::make_shared<ActionToTargetTurning>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		persistenceDurationMS,
		priority,
		extraGroupsBitMap,
		extraPreRequirementsGroupsBitMask,
		extraReplaceGroupsBitMask,
		extraOverridesGroupsBitMask,
		targetRotZDeg, targetEntity,
		isStopAtDestination,
		isInstantTurn
	);

	return action;
}

void MEMT_TurnToTargetAngle::addAction_ActionToTargetTurning(
	int persistenceDurationMS,
	int priority,
	int extraGroupsBitMap,
	int extraPreRequirementsGroupsBitMask,
	int extraReplaceGroupsBitMask,
	int extraOverridesGroupsBitMask,
	float targetRotZDeg, std::shared_ptr<base::WrappedWorldEntity> targetEntity,
	bool isStopAtDestination,
	bool isInstantTurn)
{
	if (isnan(targetRotZDeg)) {
		// This occured when the window was minimised.
		throw LogicException(LOC);
	}

	actions->addAction(buildAction_ActionToTargetTurning(
		persistenceDurationMS,
		priority,
		extraGroupsBitMap,
		extraPreRequirementsGroupsBitMask,
		extraReplaceGroupsBitMask,
		extraOverridesGroupsBitMask,
		targetRotZDeg, targetEntity,
		isStopAtDestination,
		isInstantTurn
	));
}

void MEMT_TurnToTargetAngle::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if(action.getType() == ActionToTargetTurning::TYPE) {
		onEntityAction_ActionToTargetTurning((ActionToTargetTurning*)action.getRawSubclassPointer(ActionToTargetTurning::TYPE));
	}else {
		throw LogicException(LOC);
	}
}

void MEMT_TurnToTargetAngle::onEntityAction_ActionToTargetTurning(ActionToTargetTurning* action) {
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
		e->dispatchEvent(*EVENT_turn_Start);
	}

	if (action->targetEntity != nullptr && action->targetEntity->entity == nullptr) {
		// The target entity was removed, stop the turn.
		action->persistentActionFinished();

		return;
	}

	float currentAngZDeg = entity->getRot()->getAngleAround(Vector3::Z);

	float destAngZDeg = action->targetRotZDeg;
	if (action->targetEntity != nullptr) {
		destAngZDeg += Math::atan2Deg(
			action->targetEntity->entity->getY() - e->getY(),
			action->targetEntity->entity->getX() - e->getX()
		);
	}

	float newAngZDeg;
	bool isDestinationReached = false;
	if (action->isInstantTurn) {
		newAngZDeg = destAngZDeg;

		isDestinationReached = true;
	} else {
		float tickTurnAmount = t->turnZSpeedDegS_max * action->deltaS;

		float angZDegDelta = Math::closestAngleDeg(
			currentAngZDeg, destAngZDeg
		);

		if (tickTurnAmount >= Math::abs(angZDegDelta)) {
			newAngZDeg = destAngZDeg;

			isDestinationReached = true;
		} else {
			newAngZDeg = currentAngZDeg + tickTurnAmount * Math::sign(angZDegDelta);
		}
	}
	if (isDestinationReached && action->isStopAtDestination) {
		action->persistentActionFinished();
	}

	this->entity->getPositional3D()->setCardinalRotation(newAngZDeg);
	lookAt->setLookAtAnglesDeg(
		newAngZDeg,
		lookAt->getAngDElevation()
	);
}

MEMT_TurnToTargetAngle::~MEMT_TurnToTargetAngle() {
	//void
}
