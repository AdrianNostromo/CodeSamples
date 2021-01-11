#include "SM_Pickup.h"
#include <functional>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/IActionsModule.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/math/Math.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <worldGame3D/gw/entity/util/WrappedWorldEntity.h>

using namespace rpg3D;

int SM_Pickup::StackablePickupActionContinuous::TYPE = super::NewTypeIndex();

const int SM_Pickup::StackablePickupActionContinuous::States::Start = 0;
const int SM_Pickup::StackablePickupActionContinuous::States::ManagedMoveInRange = 1;
const int SM_Pickup::StackablePickupActionContinuous::States::ManagedTurnToTarget = 2;
const int SM_Pickup::StackablePickupActionContinuous::States::Pickup = 3;
const int SM_Pickup::StackablePickupActionContinuous::States::Finished = 4;

SM_Pickup::StackablePickupActionContinuous::StackablePickupActionContinuous(
	int localEntityId, int syncedEntityId,
	int priority,
	std::shared_ptr<base::WrappedWorldEntity>& targetEntity,
	float actionRange)
	: super(
		TYPE, "StackablePickupActionContinuous",
		true/*isSyncedToServer*/, -1/*persistenceDurationMS*/,
		localEntityId, syncedEntityId,
		priority/*priority*/, 
		ActionGroup_StackablePickupOrThrow,/*groupsBitMap*/
		0/*preRequirementsGroupsBitMask*/,
		ActionGroup_StackablePickupOrThrow/*replaceGroupsBitMask*/,
		0/*overridesGroupsBitMask*/
	),
	targetEntity(targetEntity),
	actionRange(actionRange)
{
	rawSubclassPointer = this;
};

SM_Pickup::StackablePickupActionContinuous::~StackablePickupActionContinuous() {
	//void
};


SM_Pickup::SM_Pickup(
	IWorldEntity* entity,
	StackableTemplate* t)
	: super(entity, t)
{
	//void
}

void SM_Pickup::createBMain() {
	super::createBMain();

	if (t->canHaul) {
		actions = entity->getComponentAs<worldGame3D::IActionsModule*>(false/*mustExist*/);
		// Moving module is optional. An error will be dispatched if moves will be requested.
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(false/*mustExist*/);

		actions->setActionProcessor(
			StackablePickupActionContinuous::TYPE,
			std::bind(&SM_Pickup::onAction, this, std::placeholders::_1, std::placeholders::_2),
			autoListenersList
		);
	}
}

void SM_Pickup::addAction_StackablePickupActionContinuous(std::shared_ptr<base::WrappedWorldEntity> targetEntity, float actionRange) {
	actions->addAction(std::make_shared<StackablePickupActionContinuous>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		targetEntity,
		actionRange
	));
}

void SM_Pickup::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if (action.getType() == StackablePickupActionContinuous::TYPE) {
		onEntityAction_StackablePickupActionContinuous((StackablePickupActionContinuous*)action.getRawSubclassPointer(StackablePickupActionContinuous::TYPE));
	}
	
	else {
		throw LogicException(LOC);
	}
}

void SM_Pickup::onEntityAction_StackablePickupActionContinuous(StackablePickupActionContinuous* action) {
	if (action->getIsReplaced() || action->getIsFinished() || !action->getIsPreRequirementsMet()) {
		// This is the last time this action is called.
		// Do action finish,cleanup logic (eg. remove active status effects from the list).

		//asd_g;// Handle cancel. If animating step active, move to final position.
		//void
	}
	if (action->getIsReplaced() || action->getIsOverridden() || !action->getIsPreRequirementsMet()) {
		// Action replaced. This is the last time this action is called.
		// Or action is overridden. Should not do any active logic now excet for the above one in certain cases (eg. to remove active status effects from the list).

		return;
	}

	if (action->targetEntity->entity == nullptr) {
		action->persistentActionFinished();

		return;
	}

	bool isReturnRequested = false;
	int newState = action->state;
	while(true) {
		switch (action->state) {
		case StackablePickupActionContinuous::States::Start: {
			if (!action->isStateInit) {
				if (action->ticksCounter == 0) {
					e->dispatchEvent(*EVENT_stackablePickup_Start);
				}

				newState = StackablePickupActionContinuous::States::ManagedMoveInRange;
			} else {
				//void
			}

			break;
		}
		case StackablePickupActionContinuous::States::ManagedMoveInRange: {
			if (!action->isStateInit) {
				if (action->actionRange >= 0 && Math::compare(Math::dist(entity->getX(), entity->getY(), action->targetEntity->entity->getX(), action->targetEntity->entity->getY()), action->actionRange) > 0.0f) {
					// Pickup range use and out of range, need to move towards the target.
					if (movingEntity == nullptr) {
						// This entity doesn't move and must never be required to move to get in range.
						// Increase range or make it -1 is probably required.
						throw LogicException(LOC);
					}

					// Add a move to target.
					Vector2 destOffset{};
					// Rounding error requires the movement to come ~1mm closer to the target).
					std::shared_ptr<worldGame3D::EntityAction> depAction = movingEntity->buildAction_ActionToTargetMovementContinuous(
						IMovingEntityModule::ControlType::LocalEntityDirection_BackForwardOnly,
						destOffset, action->targetEntity,
						Math::max(0.0f, action->actionRange - 0.001f), true/*isStopAtDestination*/,
						true/*isAutoTurn*/, false/*isInstantTurn*/,
						moduleId
					);
					action->childDependencyActionsToPreInjectList.append_emplace(depAction, true/*addAsDependency*/, false/*addAsDependant*/, false/*triggerDependencyFailOnFinishSuccess*/, true/*triggerDependencyFailOnFail*/);

					isReturnRequested = true;
				} else {
					// Already in range.
					newState = StackablePickupActionContinuous::States::ManagedTurnToTarget;
				}
			} else {
				if (action->getDependenciesCount() == 0) {
					newState = StackablePickupActionContinuous::States::ManagedTurnToTarget;
				}
			}

			break;
		}
		case StackablePickupActionContinuous::States::ManagedTurnToTarget: {
			if (!action->isStateInit) {
				if (entity->getX() != action->targetEntity->entity->getX() || entity->getY() != action->targetEntity->entity->getY()) {
					// Add a turn to target direction action if not overlapping.
					if (movingEntity == nullptr) {
						// This entity doesn't move and must never be required to move to get in range.
						// Increase range or make it -1 is probably required.
						throw LogicException(LOC);
					}
					
					float angR = Math::atan2(action->targetEntity->entity->getY() - entity->getY(), action->targetEntity->entity->getX() - entity->getX());
					float angD = Math::toDegrees(angR);

					//asdA37;// Use inverted dependency so it is blocking until the pickup finishes.
					std::shared_ptr<worldGame3D::EntityAction> depAction = movingEntity->buildAction_ActionToTargetTurning(
						-1/*persistenceDurationMS*/,
						worldGame3D::EntityAction::Priorities::NormalPlus1/*priority*/,
						ActionGroup_StackableTurning/*extraGroupsBitMap*/,
						0/*extraPreRequirementsGroupsBitMask*/,
						IMovingEntityModule::ActionGroup_Turn | ActionGroup_StackableTurning/*extraReplaceGroupsBitMask*/,
						0/*extraOverridesGroupsBitMask*/,

						angD/*targetRotZDeg*/, nullptr/*targetEntity*/,
						false/*isStopAtDestination*/,
						false/*isInstantTurn*/
					);
					action->childDependencyActionsToPreInjectList.append_emplace(depAction, false/*addAsDependency*/, true/*addAsDependant*/, true/*triggerDependencyFailOnFinishSuccess*/, true/*triggerDependencyFailOnFail*/);

					isReturnRequested = true;
				} else {
					newState = StackablePickupActionContinuous::States::Pickup;
				}
			} else {
				float entityYawDeg = entity->getRot()->getAngleAround(Vector3::Z);
				float yawToTarget = Math::atan2Deg(action->targetEntity->entity->getY() - entity->getY(), action->targetEntity->entity->getX() - entity->getX());

				float angD = Math::closestAngleDeg(entityYawDeg, yawToTarget);
				if (Math::abs(angD) < 15.0f) {
					newState = StackablePickupActionContinuous::States::Pickup;
				}
			}

			break;
		}
		case StackablePickupActionContinuous::States::Pickup: {
			if (!action->isStateInit) {
				// If not possible (something changed), cancel the action->
				IStackableModule* targetEntity_stackable = action->targetEntity->entity->getComponentAs< IStackableModule*>(true/*mustExist*/);

				// Do a last check to avoid logic errors.
				if (action->targetEntity->entity == nullptr || getIsCarrying()) {
					// Finish will take care of the action finishing.

					//void
				} else {
					//asdA36;//Add a total stop action on the target in case it has active movements or stuff.
					//asdA36;// Maybe also do a stun_effect or a high_priority_continuous_blocking_action on the target while carried so it won't do things.
					setCarrying(
						targetEntity_stackable,
						moduleId
					);
				}
			} else {
				//asd_g;// animate;
				//void
			}

			newState = StackablePickupActionContinuous::States::Finished;

			break;
		}
		case StackablePickupActionContinuous::States::Finished: {
			if (!action->isStateInit) {
				action->persistentActionFinished();

				isReturnRequested = true;
			} else {
				//void
			}

			break;
		}
		default: {
			throw LogicException(LOC);
		}}

		if (!action->isStateInit) {
			action->isStateInit = true;
		}

		if (newState != action->state) {
			action->state = newState;
			action->isStateInit = false;
		} else {
			break;
		}

		if (isReturnRequested) {
			break;
		}
	}
}

SM_Pickup::~SM_Pickup() {
	//void
}
