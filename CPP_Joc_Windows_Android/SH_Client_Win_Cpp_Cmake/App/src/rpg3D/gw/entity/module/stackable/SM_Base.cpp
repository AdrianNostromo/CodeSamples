#include "SM_Base.h"
#include <rpg3D/gw/entity/template/stackable/StackableTemplate.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/entity/module/movingEntity/IMovingEntityModule.h>
#include <rpg3D/gw/entity/module/physics3D/IPhysics3DModule.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <rpg3D/gw/physics3D/IGamePartPhysics3D.h>
#include <rpg3D/gw/entity/module/physics3D/constraint3D/config/CfgConstraint3D6Dof.h>
#include <rpg3D/gw/entity/module/physics3D/constraint3D/IConstraint3D.h>
#include <worldGame3D/gw/entity/util/IEntityHandlerBase.h>
#include <worldGame3D/gw/IGameWorld.h>

using namespace rpg3D;

SM_Base::SM_Base(
	IWorldEntity* entity,
	StackableTemplate* t)
	: super(t->moduleName, entity),
	t(t)
{
	haulOffsetZM = t->haulOffsetZM;

	e->addEventListener(
		IMovingEntityModule::EVENT_move_Start->type,
		std::bind(&SM_Base::onEvent, this, std::placeholders::_1, std::placeholders::_2),
		autoListenersList
	);
}

void SM_Base::createBMain() {
	super::createBMain();

	physics3DModule = entity->getComponentAs<rpg3D::IPhysics3DModule*>(false/*mustExist*/);
}

StackableTemplate* SM_Base::getT() {
	return t;
}

IPhysics3DModule* SM_Base::getPhysics3DModule() {
	if (physics3DModule == nullptr) {
		throw LogicException(LOC);
	}

	return physics3DModule;
}

float SM_Base::getHaulOffsetZM() {
	return haulOffsetZM;
}

void SM_Base::setHaulOffsetZM(float haulOffsetZM) {
	if (this->haulOffsetZM == haulOffsetZM) {
		return;
	}

	this->haulOffsetZM = haulOffsetZM;

	if (carryingPhysicsConstraint != nullptr) {
		throw LogicException(LOC);
		//asdA_p;// modify the existing contraint. Implement this later or when it occurs.
	}
}

bool SM_Base::getIsHauled() {
	if (haulerStackable != nullptr) {
		return true;
	}

	return false;
}

bool SM_Base::getIsCarrying() {
	if (carryingStackable != nullptr) {
		return true;
	}

	return false;
}

bool SM_Base::getIsCarryingIndirect(IStackableModule* targetStackable) {
	IStackableModule* subStackable = carryingStackable;
	while (subStackable != nullptr) {
		if (subStackable == targetStackable) {
			return true;
		}

		subStackable = subStackable->getCarrying();
	}

	return false;
}

IStackableModule* SM_Base::getCarrying() {
	return carryingStackable;
}

void SM_Base::setCarrying(IStackableModule* carryingStackable, std::string& initiatorIdentifier) {
	if (carryingStackable != nullptr && carryingStackable->getIsHauled()) {
		// The target entity is hauled by anoter entity.
		// Need to break that hauling first.
		carryingStackable->getHauler()->setCarrying(nullptr, initiatorIdentifier);
	}

	if (this->carryingStackable != nullptr) {
		this->carryingStackable->setHauler(nullptr, initiatorIdentifier);

		if (carryingPhysicsConstraint == nullptr) {
			throw LogicException(LOC);
		}
		carryingPhysicsConstraint->removeConstraint();
		carryingPhysicsConstraint = nullptr;

		this->carryingStackable->getPhysics3DModule()->activateAllPBodies();

		this->carryingStackable = nullptr;
	}

	this->carryingStackable = carryingStackable;

	if (this->carryingStackable != nullptr) {
		// Pickup the target.
		// Also stop the movements of the target and increment the waiting_actions_cancel_index on the target.
		carryingStackable->setHauler(this, initiatorIdentifier);

		btTransform frameInA;
		frameInA = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(0.0f, 0.0f, 0.2f));

		btTransform frameInB;
		frameInB = btTransform::getIdentity();
		frameInB.setOrigin(btVector3(0.0f, 0.0f, -0.2f));

		std::shared_ptr<CfgConstraint3D6Dof> cfgConstraint(new CfgConstraint3D6Dof(
			frameInA, frameInB,
			true/*useLinearReferenceFrameA*/
		));

		carryingPhysicsConstraint = physics3DModule->createConstraint(cfgConstraint, this->carryingStackable->getPhysics3DModule());

		//asd_g;// need a method to animate this constraint.
	}

	e->dispatchEvent(*EVENT_carryingChanged->setData_initiatorIdentifier(initiatorIdentifier));
}

IStackableModule* SM_Base::getBottomStackable() {
	IStackableModule* lowerStackable = this;
	while (lowerStackable != nullptr) {
		if (lowerStackable->getHauler() == nullptr) {
			break;
		}

		lowerStackable = lowerStackable->getHauler();
	}

	return lowerStackable;
}

IStackableModule* SM_Base::getTopStackable() {
	IStackableModule* upperStackable = this;
	while (upperStackable != nullptr) {
		if (upperStackable->getCarrying() == nullptr) {
			break;
		}

		upperStackable = upperStackable->getCarrying();
	}

	return upperStackable;
}

IStackableModule* SM_Base::getHauler() {
	return haulerStackable;
}

void SM_Base::setHauler(IStackableModule* haulerStackable, std::string& initiatorIdentifier) {
	if (this->haulerStackable != nullptr) {
		this->haulerStackable = nullptr;

		// Enable movement actions.
		/*asdA24;
		MovingEntityModule movingEntity = entity-><MovingEntityModule>getModule(rpg3D::MovingEntityTemplate.MODULE_ID);
		if (movingEntity != nullptr) {
			entity-><MovingEntityModule>getModule(rpg3D::MovingEntityTemplate.MODULE_ID).enableDirectPlayerMovementActions();
		}*/
	}

	this->haulerStackable = haulerStackable;

	if (this->haulerStackable != nullptr) {
		// Stop movement if moving entity module exists.
		//asdA24;
		//MovingEntityModule movingEntity = entity-><MovingEntityModule>getModule(rpg3D::MovingEntityTemplate.MODULE_ID);
		//if (movingEntity != nullptr) {
		//	movingEntity.stopMovement();

		//	// Disable movement actions.
		//	entity-><MovingEntityModule>getModule(rpg3D::MovingEntityTemplate.MODULE_ID).disableDirectPlayerMovementActions();
		//}
	}

	e->dispatchEvent(*EVENT_haulerChanged->setData_initiatorIdentifier(initiatorIdentifier));
}

void SM_Base::onEvent(IEventListener& eventListener, base::EntityEvent& event) {
	if (event.type == IMovingEntityModule::EVENT_move_Start->type) {
		if (haulerStackable != nullptr) {
			// Jump off the stack with the carried_stackables.
			// The offset position will be automatically updated by the movement.
			setHauler(nullptr, StringUtil::empty);
		}
	}

	else {
		throw LogicException(LOC);
	}
}

void SM_Base::disposePre() {
	// Exit from the stack and splice it back.
	if (getIsHauled()) {
		if (getIsCarrying()) {
			// Splice the stack.
			haulerStackable->setCarrying(carryingStackable, StringUtil::empty);
		} else {
			// Exit from the hauler.
			haulerStackable->setCarrying(nullptr, StringUtil::empty);
		}
	} else {
		if (getIsCarrying()) {
			setCarrying(nullptr, StringUtil::empty);
		}
	}

	super::disposePre();
}

SM_Base::~SM_Base() {
	//void
}
