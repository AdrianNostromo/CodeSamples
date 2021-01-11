#include "Constraint3D.h"
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>
#include "IHandlerContraint3D.h"

using namespace rpg3D;

Constraint3D::Constraint3D(
	IHandlerContraint3D* handler, std::shared_ptr<CfgConstraint3D> cfg, btTypedConstraint* constraint_managed,
	IPhysics3DModule* otherTarget)
	: super(),
	handler(handler), cfg(cfg), constraint_managed(constraint_managed),
	otherTarget(otherTarget)
{
	//void
}

void Constraint3D::addToPWorld(btDiscreteDynamicsWorld* pWorld) {
	if (this->pWorld != nullptr) {
		throw LogicException(LOC);
	}

	this->pWorld = pWorld;

	this->pWorld->addConstraint(constraint_managed);
}

void Constraint3D::removeFromPWorld() {
	if (this->pWorld == nullptr) {
		return;
	}

	this->pWorld->removeConstraint(constraint_managed);

	this->pWorld = nullptr;
}

void Constraint3D::removeConstraint() {
	handler->removeConstraint(this);
}

void Constraint3D::disposeMain() {
	if (constraint_managed != nullptr) {
		if (pWorld != nullptr) {
			removeFromPWorld();
		}

		delete constraint_managed;
		constraint_managed = nullptr;
	}

	super::disposeMain();
}

Constraint3D::~Constraint3D() {
	reservedDisposeIfNeeded();// Note1001
}
