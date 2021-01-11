#include "FM_GeneralFlight.h"
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include "../../template/flying/FlyingTemplate.h"
#include <base/math/Math.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <worldGame3D/gw/entity/util/WrappedWorldEntity.h>

using namespace rpg3D;

int FM_GeneralFlight::ActionGroup_GeneralFlight = worldGame3D::EntityAction::GetNextAvailableGroupSingleBitMap();

int FM_GeneralFlight::ActionDirectionFlight::TYPE = super::NewTypeIndex();
int FM_GeneralFlight::ActionApplyImpulse::TYPE = super::NewTypeIndex();
int FM_GeneralFlight::ActionDestinationFlight::TYPE = super::NewTypeIndex();

FM_GeneralFlight::FM_GeneralFlight(
	IWorldEntity* entity,
	FlyingTemplate* t)
	: super(entity, t)
{
	//void
}

void FM_GeneralFlight::createBMain() {
	super::createBMain();

	actions->setActionProcessor(
		ActionDirectionFlight::TYPE, ActionApplyImpulse::TYPE, ActionDestinationFlight::TYPE,
		std::bind(&FM_GeneralFlight::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void FM_GeneralFlight::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if (action.getType() == ActionDirectionFlight::TYPE) {
		onEntityAction_ActionDirectionFlight((ActionDirectionFlight*)action.getRawSubclassPointer(ActionDirectionFlight::TYPE));
	} else if (action.getType() == ActionApplyImpulse::TYPE) {
		onEntityAction_ActionApplyImpulse((ActionApplyImpulse*)action.getRawSubclassPointer(ActionApplyImpulse::TYPE));
	} else if (action.getType() == ActionDestinationFlight::TYPE) {
		onEntityAction_ActionDestinationFlight((ActionDestinationFlight*)action.getRawSubclassPointer(ActionDestinationFlight::TYPE));
	} else {
		throw LogicException(LOC);
	}
}

void FM_GeneralFlight::addAction_ActionDirectionFlight(Vector3& velocityDir) {
	velocityDir.clipLength(1.0f);

	actions->addAction(std::make_shared<ActionDirectionFlight>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		velocityDir
	));
}

void FM_GeneralFlight::onEntityAction_ActionDirectionFlight(ActionDirectionFlight* action) {
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

	Vector3* cPos = entity->getPos();
	entity->setPos(
		cPos->x + action->velocityDir.x * t->flyingSpeedS * action->deltaS,
		cPos->y + action->velocityDir.y * t->flyingSpeedS * action->deltaS,
		cPos->z + action->velocityDir.z * t->flyingSpeedS * action->deltaS
	);
}

void FM_GeneralFlight::addAction_ActionApplyImpulse(
	Vector3& dir,
	float physicsImpulse)
{
	dir.nor();

	actions->addAction(std::make_shared<ActionApplyImpulse>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		dir, physicsImpulse
	));
}

void FM_GeneralFlight::onEntityAction_ActionApplyImpulse(ActionApplyImpulse* action) {
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

	if (physics3DModule == nullptr) {
		// This action requires a physics module.
		throw LogicException(LOC);
	}

	physics3DModule->applyImpulse(
		action->dir.x * action->physicsImpulse,
		action->dir.y * action->physicsImpulse,
		action->dir.z * action->physicsImpulse
	);
}

void FM_GeneralFlight::addAction_ActionDestinationFlight(
	IWorldEntity* targetEntity, Vector3& targetPosOrGlobalOffset,
	bool flyToLastKnownTargetPosOnTargetLoss, bool stopFlyingAtDestination)
{
	if (targetEntity == nullptr) {
		throw LogicException(LOC);
	}

	Vector3 p{};
	if (targetEntity != nullptr) {
		p.set(targetEntity->getPos());
	}

	actions->addAction(std::make_shared<ActionDestinationFlight>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		targetEntity->getWrapper(), targetPosOrGlobalOffset,
		p,
		flyToLastKnownTargetPosOnTargetLoss, stopFlyingAtDestination
	));
}

void FM_GeneralFlight::onEntityAction_ActionDestinationFlight(ActionDestinationFlight* action) {
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

	if (action->targetEntity != nullptr) {
		if (action->targetEntity->entity == nullptr) {
			if (!action->flyToLastKnownTargetPosOnTargetLoss) {
				// Cancel the flight.
				e->dispatchEvent(*EVENT_FlyingFinished_TargetLost);
				
				action->persistentActionFinished();

				return;
			}
		}else {
			action->cachedTargetEntityPos.set(action->targetEntity->entity->getPos());
		}
	}

	Vector3 bakedDestPos{ action->targetPosOrGlobalOffset };
	bakedDestPos.add(action->cachedTargetEntityPos);

	float tickMoveAmount = t->flyingSpeedS * action->deltaS;

	Vector3* cPos = entity->getPos();

	float distToTarget = Math::dist(
		cPos->x, cPos->y, cPos->z,
		bakedDestPos.x, bakedDestPos.y, bakedDestPos.z
	);

	if (tickMoveAmount >= distToTarget) {
		// Destination reached.
		entity->setPos(bakedDestPos);

		if (action->stopFlyingAtDestination) {
			e->dispatchEvent(*EVENT_FlyingFinished_DestinationReached);
			
			action->persistentActionFinished();

			return;
		} else if (action->targetEntity != nullptr && action->targetEntity->entity == nullptr) {
			e->dispatchEvent(*EVENT_FlyingFinished_TargetLost);

			action->persistentActionFinished();

			return;
		}
	} else {
		Vector3 targetDir{
			bakedDestPos.x - cPos->x,
			bakedDestPos.y - cPos->y,
			bakedDestPos.z - cPos->z
		};
		targetDir.nor();

		entity->setPos(
			cPos->x + targetDir.x * t->flyingSpeedS * action->deltaS,
			cPos->y + targetDir.y * t->flyingSpeedS * action->deltaS,
			cPos->z + targetDir.z * t->flyingSpeedS * action->deltaS
		);
	}
}

FM_GeneralFlight::~FM_GeneralFlight() {
	//void
}
