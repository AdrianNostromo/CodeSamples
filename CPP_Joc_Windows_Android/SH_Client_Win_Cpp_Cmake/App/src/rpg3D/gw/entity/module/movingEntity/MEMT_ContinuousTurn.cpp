#include "MEMT_ContinuousTurn.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <worldGame3D/gw/entity/module/lookAt/LookAtModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/math/Math.h>
#include <base/ease/Ease.h>

using namespace rpg3D;

int MEMT_ContinuousTurn::ActionDirectionalTurnZContinuous::TYPE = super::NewTypeIndex();

const int MEMT_ContinuousTurn::TURN_REQUEST_TYPE_Directional = newTurnRequestType();

const float MEMT_ContinuousTurn::MaxTurnSpeedZDegMax = 90.0f;

MEMT_ContinuousTurn::MEMT_ContinuousTurn(
	IWorldEntity* entity,
	rpg3D::MovingEntityTemplate* t)
	: super(entity, t)
{
	//void
}

void MEMT_ContinuousTurn::createBMain() {
	super::createBMain();

	actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);

	lookAt = entity->getComponentAs<ILookAtModule*>(true/*mustExist*/);

	actions->setActionProcessor(
		ActionDirectionalTurnZContinuous::TYPE,
		std::bind(&MEMT_ContinuousTurn::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void MEMT_ContinuousTurn::addAction_ActionDirectionalTurnZContinuous(
	Vector1& dir)
{
	Vector1 v{dir};
	v.clipLength(1.0f);// Make sure the length is <=1.0f;

	actions->addAction(std::make_shared<ActionDirectionalTurnZContinuous>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		v
	));
}

void MEMT_ContinuousTurn::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if(action.getType() == ActionDirectionalTurnZContinuous::TYPE) {
		onEntityAction_ActionDirectionalTurnZContinuous((ActionDirectionalTurnZContinuous*)action.getRawSubclassPointer(ActionDirectionalTurnZContinuous::TYPE));
	}else {
		throw LogicException(LOC);
	}
}

void MEMT_ContinuousTurn::onEntityAction_ActionDirectionalTurnZContinuous(ActionDirectionalTurnZContinuous* action) {
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

	if (action->dir.x == 0.0f) {
		if (action->ticksCounter == 0) {
			e->dispatchEvent(*EVENT_turn_Stop);
		}

		action->persistentActionFinished();
	} else {
		if (action->ticksCounter == 0) {
			e->dispatchEvent(*EVENT_turn_Start);
		}

		float currentAngZDeg = entity->getRot()->getAngleAround(Vector3::Z);

		// Use a hard limit here of about 180 degrees per second.
		float tickTurnAmount = Math::min(t->turnZSpeedDegS_max, MaxTurnSpeedZDegMax) * action->deltaS;

		Vector1 globalTurnDir{};
		globalTurnDir.set(action->dir);
		globalTurnDir.clipLength(1.0f);

		float newAngZDeg = currentAngZDeg + globalTurnDir.x * tickTurnAmount;

		entity->setRot(Vector3::Z, newAngZDeg);
		lookAt->setLookAtAnglesDeg(
			newAngZDeg,
			lookAt->getAngDElevation()
		);
	}
}

MEMT_ContinuousTurn::~MEMT_ContinuousTurn() {
	//void
}
