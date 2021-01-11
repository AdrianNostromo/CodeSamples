#include "SM_Throw.h"
#include <functional>
#include <worldGame3D/gw/entity/module/actions/IActionsModule.h>
#include <worldGame3D/gw/entity/template/actions/ActionsTemplate.h>
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <base/math/Math.h>
#include <rpg3D/gw/entity/template/movingEntity/MovingEntityTemplate.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>

using namespace rpg3D;

int SM_Throw::StackableThrowActionContinuous::TYPE = super::NewTypeIndex();

const int SM_Throw::StackableThrowActionContinuous::States::Start = 0;
const int SM_Throw::StackableThrowActionContinuous::States::ManagedMoveInRange = 1;
const int SM_Throw::StackableThrowActionContinuous::States::ManagedTurnToTarget = 2;
const int SM_Throw::StackableThrowActionContinuous::States::Throw = 3;
const int SM_Throw::StackableThrowActionContinuous::States::Finished = 4;

SM_Throw::StackableThrowActionContinuous::StackableThrowActionContinuous(
	int localEntityId, int syncedEntityId,
	int priority,
	std::shared_ptr<base::WrappedWorldEntity>& carriedEntityToThrow,
	std::shared_ptr<base::WrappedWorldEntity>& throwDestinationHaulerEntity, std::shared_ptr<Vector3>& throwPosOrOffset,
	float actionRange,
	std::shared_ptr<float>& throwRotZDeg)
	: super(
		TYPE, "StackableThrowActionContinuous",
		true/*isSyncedToServer*/, -1/*persistenceDurationMS*/,
		localEntityId, syncedEntityId,
		priority/*priority*/, 
		ActionGroup_StackablePickupOrThrow,/*groupsBitMap*/
		0/*preRequirementsGroupsBitMask*/,
		ActionGroup_StackablePickupOrThrow/*replaceGroupsBitMask*/,
		0/*overridesGroupsBitMask*/
	),
	carriedEntityToThrow(carriedEntityToThrow),
	throwDestinationHaulerEntity(throwDestinationHaulerEntity), throwPosOrOffset(throwPosOrOffset),
	actionRange(actionRange),
	throwRotZDeg(throwRotZDeg)
{
	rawSubclassPointer = this;
};

SM_Throw::StackableThrowActionContinuous::~StackableThrowActionContinuous() {
	//void
}

SM_Throw::SM_Throw(
	IWorldEntity* entity,
	StackableTemplate* t)
	: super(entity, t)
{
	//void
}

void SM_Throw::createBMain() {
	super::createBMain();

	if(t->canHaul) {
		actions = entity->getComponentAs<worldGame3D::IActionsModule*>(false/*mustExist*/);
		// Moving module is optional. An error will be dispatched if moves will be requested.
		movingEntity = entity->getComponentAs<IMovingEntityModule*>(false/*mustExist*/);

		actions->setActionProcessor(
			StackableThrowActionContinuous::TYPE,
			std::bind(&SM_Throw::onAction, this, std::placeholders::_1, std::placeholders::_2),
			autoListenersList
		);
	}
}

void SM_Throw::addAction_StackableThrowActionContinuous(
	std::shared_ptr<base::WrappedWorldEntity>& carriedEntityToThrow,
	std::shared_ptr<base::WrappedWorldEntity> throwDestinationHaulerEntity, std::shared_ptr<Vector3> throwPosOrOffset,
	float throwRange,
	std::shared_ptr<float> throwRotZDeg)
{
	actions->addAction(std::make_shared<StackableThrowActionContinuous>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		carriedEntityToThrow,
		throwDestinationHaulerEntity, throwPosOrOffset,
		throwRange,
		throwRotZDeg
	));
}

void SM_Throw::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if (action.getType() == StackableThrowActionContinuous::TYPE) {
		onEntityAction_StackableThrowActionContinuous((StackableThrowActionContinuous*)action.getRawSubclassPointer(StackableThrowActionContinuous::TYPE));
	}

	else {
		throw LogicException(LOC);
	}
}

void SM_Throw::onEntityAction_StackableThrowActionContinuous(StackableThrowActionContinuous* action) {
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

	if (action->carriedEntityToThrow->entity == nullptr || (action->throwDestinationHaulerEntity != nullptr && action->throwDestinationHaulerEntity->entity == nullptr)) {
		// The target entity was removed or the destination entity was used and removed, stop the movement.
		action->persistentActionFinished();

		return;
	}

	Vector3 bakedThrowDestinationPos{};
	if (action->throwDestinationHaulerEntity != nullptr) {
		bakedThrowDestinationPos.add(action->throwDestinationHaulerEntity->entity->getPos());
	}
	if (action->throwPosOrOffset != nullptr) {
		bakedThrowDestinationPos.add(*action->throwPosOrOffset);
	}

	bool isReturnRequested = false;
	int newState = action->state;
	while (true) {
		switch (action->state) {
		case StackableThrowActionContinuous::States::Start: {
			if (!action->isStateInit) {
				if (action->ticksCounter == 0) {
					e->dispatchEvent(*EVENT_stackableThrow_Start);
				}

				newState = StackableThrowActionContinuous::States::ManagedMoveInRange;
			} else {
				//void
			}

			break;
		}
		case StackableThrowActionContinuous::States::ManagedMoveInRange: {
			if (!action->isStateInit) {
				if(action->actionRange >= 0 && Math::compare(Math::dist(entity->getX(), entity->getY(), bakedThrowDestinationPos.x, bakedThrowDestinationPos.y), action->actionRange) > 0.0f) {
					// Pickup range use and out of range, need to move towards the target.
					if (movingEntity == nullptr) {
						// This entity doesn't move and must never be required to move to get in range.
						// Increase range or make it -1 is probably required.
						throw LogicException(LOC);
					}

					// Add a move to target.
					Vector2 destPosOrOffset{};
					if (action->throwPosOrOffset != nullptr) {
						destPosOrOffset.set(action->throwPosOrOffset->x, action->throwPosOrOffset->y);
					}
					// Rounding error requires the movement to come ~1mm closer to the target).
					std::shared_ptr<worldGame3D::EntityAction> depAction = movingEntity->buildAction_ActionToTargetMovementContinuous(
						IMovingEntityModule::ControlType::LocalEntityDirection_BackForwardOnly,
						destPosOrOffset, action->throwDestinationHaulerEntity,
						Math::max(0.0f, action->actionRange - 0.001f), true/*isStopAtDestination*/,
						true, false,
						moduleId
					);
					action->childDependencyActionsToPreInjectList.append_emplace(depAction, true/*addAsDependency*/, false/*addAsDependant*/, false/*triggerDependencyFailOnFinishSuccess*/, true/*triggerDependencyFailOnFail*/);

					isReturnRequested = true;
				} else {
					// Already in range.
					newState = StackableThrowActionContinuous::States::ManagedTurnToTarget;
				}
			} else {
				if (action->getDependenciesCount() == 0) {
					newState = StackableThrowActionContinuous::States::ManagedTurnToTarget;
				}
			}

			break;
		}
		case StackableThrowActionContinuous::States::ManagedTurnToTarget: {
			if (!action->isStateInit) {
				// Add a turn to target direction action if not overlapping.
				if (!Math::isEqual(bakedThrowDestinationPos.y, entity->getY()) || !Math::isEqual(bakedThrowDestinationPos.x, entity->getX())) {
					if (movingEntity == nullptr) {
						// This entity doesn't move and must never be required to move to get in range.
						// Increase range or make it -1 is probably required.
						throw LogicException(LOC);
					}

					float angR = Math::atan2(bakedThrowDestinationPos.y - entity->getY(), bakedThrowDestinationPos.x - entity->getX());
					float angD = Math::toDegrees(angR);

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
					newState = StackableThrowActionContinuous::States::Throw;
				}
			} else {
				float entityYawDeg = entity->getRot()->getAngleAround(Vector3::Z);
				float yawToTarget = Math::atan2Deg(bakedThrowDestinationPos.y - entity->getY(), bakedThrowDestinationPos.x - entity->getX());

				float angD = Math::closestAngleDeg(entityYawDeg, yawToTarget);
				if (Math::abs(angD) < 15.0f) {
					newState = StackableThrowActionContinuous::States::Throw;
				}
			}

			break;
		}
		case StackableThrowActionContinuous::States::Throw: {
			if (!action->isStateInit) {
				IStackableModule* carriedEntityToThrow_stackable = action->carriedEntityToThrow->entity->getComponentAs< IStackableModule*>(true/*mustExist*/);
				if (!getIsCarryingIndirect(carriedEntityToThrow_stackable)) {
					// Not carrying the entity any more.
					//Cancel the action->

					// Finish will take care of the action finishing.

					//void
				} else {
					// Do the actual throw logic.
					if (action->throwPosOrOffset != nullptr) {
						carriedEntityToThrow_stackable->getHauler()->setCarrying(nullptr);

						//asd_g_t; action->carriedEntityToThrow->entity->setPos(bakedThrowDestinationPos);

						if (action->throwRotZDeg != nullptr) {
							//asd_g_t; action->carriedEntityToThrow->entity->setRot(Vector3::Z, *action->throwRotZDeg);
						}
					} else {
						// A destination hauler entity exists and no offset.
						IStackableModule* throwDestinationHaulerEntity_stackable = action->throwDestinationHaulerEntity->entity->getComponentAs< IStackableModule*>(true/*mustExist*/);
						throwDestinationHaulerEntity_stackable->getTopStackable()->setCarrying(carriedEntityToThrow_stackable);
					}
				}

				newState = StackableThrowActionContinuous::States::Finished;
			} else {
				//void
			}

			break;
		}
		case StackableThrowActionContinuous::States::Finished: {
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

SM_Throw::~SM_Throw() {
	//void
}
