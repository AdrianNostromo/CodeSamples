#include "MEMT_InstantTurn.h"
#include <base/math/util/BoolUtil.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include <worldGame3D/gw/entity/module/lookAt/LookAtModule.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/entity/util/IPositional3DHandler.h>

using namespace rpg3D;

int MEMT_InstantTurn::ActionDirectRotateZElevationDelta::TYPE = super::NewTypeIndex();
int MEMT_InstantTurn::ActionDirectRotateZ::TYPE = super::NewTypeIndex();
int MEMT_InstantTurn::ActionDirectRotateZElevation::TYPE = super::NewTypeIndex();
int MEMT_InstantTurn::ActionDirectRotateElevation::TYPE = super::NewTypeIndex();

MEMT_InstantTurn::MEMT_InstantTurn(
	IWorldEntity* entity,
	rpg3D::MovingEntityTemplate* t)
	: super(entity, t)
{
	//void
}

void MEMT_InstantTurn::createBMain() {
	super::createBMain();

	actions = entity->getComponentAs<worldGame3D::IActionsModule*>(true/*mustExist*/);

	lookAt = entity->getComponentAs<ILookAtModule*>(true/*mustExist*/);
	lookAt->setLookAtAnglesDeg(
		entity->getRot()->getAngleAround(Vector3::Z),
		0.0f
	);

	actions->setActionProcessor(
		ActionDirectRotateZElevationDelta::TYPE, ActionDirectRotateZ::TYPE, ActionDirectRotateZElevation::TYPE,
		std::bind(&MEMT_InstantTurn::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
	actions->setActionProcessor(
		ActionDirectRotateElevation::TYPE,
		std::bind(&MEMT_InstantTurn::onAction, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void MEMT_InstantTurn::addAction_ActionDirectRotateZElevationDelta(float rotZDegDelta, float rotElevationDelta) {
	actions->addAction(std::make_shared<ActionDirectRotateZElevationDelta>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		rotZDegDelta, rotElevationDelta
	));
}

void MEMT_InstantTurn::addAction_ActionDirectRotateZ(int priority, float rotZDeg) {
	actions->addAction(std::make_shared<ActionDirectRotateZ>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		priority,
		rotZDeg
	));
}

void MEMT_InstantTurn::addAction_ActionDirectRotateZElevation(float rotZDeg, float rotElevationDeg) {
	actions->addAction(std::make_shared<ActionDirectRotateZElevation>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		rotZDeg, rotElevationDeg
	));
}

void MEMT_InstantTurn::addAction_ActionDirectRotateElevation(float rotElevationDeg) {
	actions->addAction(std::make_shared<ActionDirectRotateElevation>(
		entity->getLocalEntityId(), entity->getSyncedEntityId(),
		worldGame3D::EntityAction::Priorities::Normal,
		rotElevationDeg
	));
}

void MEMT_InstantTurn::onAction(IEventListener& eventListener, worldGame3D::IAction& action) {
	if(action.getType() == ActionDirectRotateZElevationDelta::TYPE) {
		onEntityAction_ActionDirectRotateZElevationDelta((ActionDirectRotateZElevationDelta*)action.getRawSubclassPointer(ActionDirectRotateZElevationDelta::TYPE));
	}else if(action.getType() == ActionDirectRotateZ::TYPE) {
		onEntityAction_ActionDirectRotateZ((ActionDirectRotateZ*)action.getRawSubclassPointer(ActionDirectRotateZ::TYPE));
	}else if(action.getType() == ActionDirectRotateZElevation::TYPE) {
		onEntityAction_ActionDirectRotateZElevation((ActionDirectRotateZElevation*)action.getRawSubclassPointer(ActionDirectRotateZElevation::TYPE));
	}else if(action.getType() == ActionDirectRotateElevation::TYPE) {
		onEntityAction_ActionDirectRotateElevation((ActionDirectRotateElevation*)action.getRawSubclassPointer(ActionDirectRotateElevation::TYPE));
	}else {
		throw LogicException(LOC);
	}
}

void MEMT_InstantTurn::onEntityAction_ActionDirectRotateZElevationDelta(ActionDirectRotateZElevationDelta* action) {
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

	float newAngDegZ = entity->getRot()->getAngleAround(Vector3::Z) + action->rotZDegDelta;
	this->entity->getPositional3D()->setCardinalRotation(newAngDegZ);
	
	lookAt->setLookAtAnglesDeg(
		newAngDegZ,
		lookAt->getAngDElevation() + action->rotElevationDelta
	);
}

void MEMT_InstantTurn::onEntityAction_ActionDirectRotateZ(ActionDirectRotateZ* action) {
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

	this->entity->setRot(
		Vector3::Z,
		action->rotZDeg
	);
}

void MEMT_InstantTurn::onEntityAction_ActionDirectRotateZElevation(ActionDirectRotateZElevation* action) {
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

	this->entity->setRot(
		Vector3::Z,
		action->rotZDeg
	);

	lookAt->setLookAtAnglesDeg(
		action->rotZDeg,
		action->rotElevationDeg
	);
}

void MEMT_InstantTurn::onEntityAction_ActionDirectRotateElevation(ActionDirectRotateElevation* action) {
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

	lookAt->setLookAtAnglesDeg(
		entity->getRot()->getAngleAround(Vector3::Z),
		action->rotElevationDeg
	);
}

void MEMT_InstantTurn::disposePre() {
	//void

	super::disposePre();
}

MEMT_InstantTurn::~MEMT_InstantTurn() {
	//void
}
