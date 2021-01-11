#include <base/math/util/BoolUtil.h>
#include <base/math/Math.h>
#include "MEMM_BasicMovements.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <worldGame3D/gw/entity/module/lookAt/LookAtModule.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigDirect.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsPush.h>
#include <rpg3D/gw/entity/template/movingEntity/movementConfig/MovementConfigPhysicsRollingSphere.h>

int MEMM_BasicMovements::ActionTeleportToDestination::TYPE = super::NewTypeIndex();
int MEMM_BasicMovements::ActionStopMovement::TYPE = super::NewTypeIndex();
int MEMM_BasicMovements::ActionGeneralStop::TYPE = super::NewTypeIndex();
int MEMM_BasicMovements::ActionCustomStop::TYPE = super::NewTypeIndex();

MEMM_BasicMovements::ActionTeleportToDestination::ActionTeleportToDestination(
	int localEntityId, int syncedEntityId,
	int priority, 
	Vector3& destPos)
	: super(
		TYPE, "ActionTeleportToDestination",
		true/*isSyncedToServer*/, 0/*persistenceDurationMS*/,
		localEntityId, syncedEntityId,
		priority/*priority*/, 
		0,/*groupsBitMap*/
		0/*preRequirementsGroupsBitMask*/,
		0/*replaceGroupsBitMask*/,
		0/*overridesGroupsBitMask*/
	),
	destPos(destPos)
{
	rawSubclassPointer = this;
}

MEMM_BasicMovements::ActionTeleportToDestination::~ActionTeleportToDestination() {
	//void
}

MEMM_BasicMovements::ActionStopMovement::ActionStopMovement(
	int localEntityId, int syncedEntityId,
	int priority,
	bool stopPhysicsLinearVelocitiesAlso)
	: super(
		TYPE, "ActionStopMovement",
		true/*isSyncedToServer*/, 0/*persistenceDurationMS*/,
		localEntityId, syncedEntityId,
		priority/*priority*/, 
		0,/*groupsBitMap*/
		0/*preRequirementsGroupsBitMask*/,
		IMovingEntityModule::ActionGroup_Move | IMovingEntityModule::ActionGroup_Turn/*replaceGroupsBitMask*/,
		0/*overridesGroupsBitMask*/
	),
	stopPhysicsLinearVelocitiesAlso(stopPhysicsLinearVelocitiesAlso)
{
	rawSubclassPointer = this;
}

MEMM_BasicMovements::ActionStopMovement::~ActionStopMovement() {
	//void
}

MEMM_BasicMovements::ActionGeneralStop::ActionGeneralStop(
	int localEntityId, int syncedEntityId,
	int priority)
	: super(
		TYPE, "ActionGeneralStop",
		true/*isSyncedToServer*/, 0/*persistenceDurationMS*/,
		localEntityId, syncedEntityId,
		priority/*priority*/, 
		0,/*groupsBitMap*/
		0/*preRequirementsGroupsBitMask*/,
		0/*replaceGroupsBitMask*/,
		0/*overridesGroupsBitMask*/
	)
{
	rawSubclassPointer = this;
}

MEMM_BasicMovements::ActionGeneralStop::~ActionGeneralStop() {
	//void
}

MEMM_BasicMovements::ActionCustomStop::ActionCustomStop(
	int localEntityId, int syncedEntityId,
	int priority,
	int extraReplaceGroupsBitMask)
	: super(
		TYPE, "ActionCustomStop",
		true/*isSyncedToServer*/, 0/*persistenceDurationMS*/,
		localEntityId, syncedEntityId,
		priority/*priority*/, 
		0,/*groupsBitMap*/
		0/*preRequirementsGroupsBitMask*/,
		0 | extraReplaceGroupsBitMask/*replaceGroupsBitMask*/,
		0/*overridesGroupsBitMask*/
	)
{
	rawSubclassPointer = this;
}

MEMM_BasicMovements::ActionCustomStop::~ActionCustomStop() {
	//void
}

MEMM_BasicMovements::MEMM_BasicMovements(
    IWorldEntity* entity,
    rpg3D::MovingEntityTemplate* t
)
    : super(entity, t)
{
	//void
}

void MEMM_BasicMovements::createBMain() {
    super::createBMain();

    actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);

    lookAt = entity->getComponentAs<ILookAtModule*>(true/*mustExist*/);
	
	physics3DModule = entity->getComponentAs<rpg3D::IPhysics3DModule*>(false/*mustExist*/);

    actions->setActionProcessor(
        ActionTeleportToDestination::TYPE, ActionStopMovement::TYPE, ActionGeneralStop::TYPE,
        std::bind(&MEMM_BasicMovements::onAction, this, std::placeholders::_1, std::placeholders::_2),
        autoListenersList
    );
    actions->setActionProcessor(
		ActionCustomStop::TYPE,
        std::bind(&MEMM_BasicMovements::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
    );
}

void MEMM_BasicMovements::addAction_teleportToDestination(Vector3& destPos) {
    actions->addAction(std::make_shared<ActionTeleportToDestination>(
        entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
        destPos
    ));
}

void MEMM_BasicMovements::addAction_stopMovement(bool stopPhysicsLinearVelocitiesAlso) {
    actions->addAction(std::make_shared<ActionStopMovement>(
        entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		stopPhysicsLinearVelocitiesAlso
    ));
}

void MEMM_BasicMovements::addAction_ActionGeneralStop() {
    actions->addAction(std::make_shared<ActionGeneralStop>(
        entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::High
    ));
}

void MEMM_BasicMovements::addAction_ActionCustomStop(
	int priority,
	int extraReplaceGroupsBitMask)
{
    actions->addAction(std::make_shared<ActionCustomStop>(
        entity->getLocalEntityId(), entity->getSyncedEntityId(),
		priority,
		extraReplaceGroupsBitMask
    ));
}

void MEMM_BasicMovements::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
    if(action.getType() == ActionTeleportToDestination::TYPE) {
        onEntityAction_ActionTeleportToDestination((ActionTeleportToDestination*)action.getRawSubclassPointer(ActionTeleportToDestination::TYPE));
    }else if(action.getType() == ActionStopMovement::TYPE) {
        onEntityAction_ActionStopMovement((ActionStopMovement*)action.getRawSubclassPointer(ActionStopMovement::TYPE));
    }else if(action.getType() == ActionGeneralStop::TYPE) {
        onEntityAction_ActionGeneralStop((ActionGeneralStop*)action.getRawSubclassPointer(ActionGeneralStop::TYPE));
    }else if(action.getType() == ActionCustomStop::TYPE) {
        onEntityAction_ActionCustomStop((ActionCustomStop*)action.getRawSubclassPointer(ActionCustomStop::TYPE));
    }else {
        throw LogicException(LOC);
    }
}

void MEMM_BasicMovements::onEntityAction_ActionTeleportToDestination(ActionTeleportToDestination* action) {
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

	e->dispatchEvent(*EVENT_teleport_Start);

	this->entity->setPos(
		action->destPos
	);
}

void MEMM_BasicMovements::onEntityAction_ActionStopMovement(ActionStopMovement* action) {
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

	rpg3D::MovementConfig* movementConfig = t->getDefaultMovementConfigMustExist();
	if (movementConfig->getType() == rpg3D::MovementConfigPhysicsPush::TYPE) {
		rpg3D::MovementConfigPhysicsPush* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigPhysicsPush*>(rpg3D::MovementConfigPhysicsPush::TYPE);

		if (action->stopPhysicsLinearVelocitiesAlso) {
			physics3DModule->setLinearForce(
				0.0f, 0.0f, 0.0f,
				false/*isContinuousUpdate*/, &cMovementConfig->stopDamping,
				false/*applyToMovementPBodyOnly*/,
				false/*useCentralForceInsteadOfVelocitySet*/
			);
		}
	} else if (movementConfig->getType() == rpg3D::MovementConfigPhysicsRollingSphere::TYPE) {
		rpg3D::MovementConfigPhysicsRollingSphere* cMovementConfig = movementConfig->castAs<rpg3D::MovementConfigPhysicsRollingSphere*>(rpg3D::MovementConfigPhysicsRollingSphere::TYPE);
		
		physics3DModule->setAngularVelocity(
			0.0f, 0.0f, 0.0f,
			false/*isContinuousUpdate*/, &cMovementConfig->stopAngularDamping
		);

		if (action->stopPhysicsLinearVelocitiesAlso) {
			physics3DModule->setLinearForce(
				0.0f, 0.0f, 0.0f,
				false/*isContinuousUpdate*/, nullptr/*linearDamping*/,
				false/*applyToMovementPBodyOnly*/,
				false/*useCentralForceInsteadOfVelocitySet*/
				);
		}
	} else if (movementConfig->getType() == rpg3D::MovementConfigDirect::TYPE) {
		//void
	} else {
		throw LogicException(LOC);
	}

	e->dispatchEvent(*EVENT_move_Stop);
}

void MEMM_BasicMovements::onEntityAction_ActionGeneralStop(ActionGeneralStop* action) {
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

	// The stopping is handled by the action stop_groups_mask.

	//void
}

void MEMM_BasicMovements::onEntityAction_ActionCustomStop(ActionCustomStop* action) {
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

	// The stopping is handled by the action stop_groups_mask.

	//void
}

void MEMM_BasicMovements::disposePre() {
    //void

    super::disposePre();
}

MEMM_BasicMovements::~MEMM_BasicMovements() {
    //void
}
