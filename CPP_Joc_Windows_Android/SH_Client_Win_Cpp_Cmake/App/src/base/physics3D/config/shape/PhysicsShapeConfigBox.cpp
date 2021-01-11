#include "PhysicsShapeConfigBox.h"

using namespace physics3D;

int PhysicsShapeConfigBox::TYPE = super::NewType();

PhysicsShapeConfigBox::PhysicsShapeConfigBox(
	float collisionMargin,
	Vector3 halfExtents)
	: super(TYPE),
	collisionMargin(collisionMargin),
	halfExtents(halfExtents)
{
	rawSubclassPointer = this;
}

PhysicsShapeConfigBox::~PhysicsShapeConfigBox() {
	//void
}
