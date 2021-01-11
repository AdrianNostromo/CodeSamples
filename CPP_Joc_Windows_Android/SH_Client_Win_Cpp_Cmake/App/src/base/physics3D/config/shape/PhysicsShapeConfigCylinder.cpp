#include "PhysicsShapeConfigCylinder.h"

using namespace physics3D;

int PhysicsShapeConfigCylinder::TYPE = super::NewType();

PhysicsShapeConfigCylinder::PhysicsShapeConfigCylinder(
	float collisionMargin,
	Vector3& mainAxis,
	Vector3 halfExtents)
	: super(TYPE),
	collisionMargin(collisionMargin),
	mainAxis(mainAxis),
	halfExtents(halfExtents)
{
	rawSubclassPointer = this;
}

PhysicsShapeConfigCylinder::~PhysicsShapeConfigCylinder() {
	//void
}
