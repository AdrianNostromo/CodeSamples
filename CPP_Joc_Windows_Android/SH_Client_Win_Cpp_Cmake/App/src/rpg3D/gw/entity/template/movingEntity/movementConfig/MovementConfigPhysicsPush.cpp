#include "MovementConfigPhysicsPush.h"

using namespace rpg3D;

const int MovementConfigPhysicsPush::TYPE = super::NewType();

MovementConfigPhysicsPush::MovementConfigPhysicsPush(
	float moveForce, float moveDamping,
	float stopDamping,
	bool applyForceToMovementPBodyOnly)
	: super(TYPE),
	moveForce(moveForce), moveDamping(moveDamping),
	stopDamping(stopDamping),
	applyForceToMovementPBodyOnly(applyForceToMovementPBodyOnly)
{
	rawSubclassPointer = this;
}

MovementConfigPhysicsPush::~MovementConfigPhysicsPush() {
	//void
}
