#include "PhysicsShapeConfigCapsule.h"

using namespace physics3D;

int PhysicsShapeConfigCapsule::TYPE = super::NewType();

PhysicsShapeConfigCapsule::PhysicsShapeConfigCapsule(
	Vector3& mainAxis,
	float endDomesRadius, float cylinderPartHeight)
	: super(TYPE),
	mainAxis(mainAxis),
	endDomesRadius(endDomesRadius), cylinderPartHeight(cylinderPartHeight)
{
	rawSubclassPointer = this;
}

PhysicsShapeConfigCapsule::~PhysicsShapeConfigCapsule() {
	//void
}
