#include "PhysicsShapeConfigSphere.h"

using namespace physics3D;

int PhysicsShapeConfigSphere::TYPE = super::NewType();

PhysicsShapeConfigSphere::PhysicsShapeConfigSphere(float radius)
	: super(TYPE),
	radius(radius)
{
	rawSubclassPointer = this;
}

PhysicsShapeConfigSphere::~PhysicsShapeConfigSphere() {
	//void
}
