#include "MovementConfigPhysicsRollingSphere.h"

using namespace rpg3D;

const int MovementConfigPhysicsRollingSphere::TYPE = super::NewType();

MovementConfigPhysicsRollingSphere::MovementConfigPhysicsRollingSphere(
	float moveAngularVelocity, float moveAngularDamping,
	float stopAngularDamping)
	: super(TYPE),
	moveAngularVelocity(moveAngularVelocity), moveAngularDamping(moveAngularDamping),
	stopAngularDamping(stopAngularDamping)
{
	rawSubclassPointer = this;
}

MovementConfigPhysicsRollingSphere::~MovementConfigPhysicsRollingSphere() {
	//void
}
