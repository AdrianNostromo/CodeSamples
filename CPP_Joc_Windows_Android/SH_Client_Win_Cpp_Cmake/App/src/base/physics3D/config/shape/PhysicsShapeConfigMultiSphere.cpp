#include "PhysicsShapeConfigMultiSphere.h"

using namespace physics3D;

int PhysicsShapeConfigMultiSphere::TYPE = super::NewType();

PhysicsShapeConfigMultiSphere::SphereInfo::SphereInfo(Vector3 position, float radius)
	: position(position), radius(radius)
{
	//void
}

PhysicsShapeConfigMultiSphere::PhysicsShapeConfigMultiSphere(int numSpheres, SphereInfo* sphereInfos)
	: super(TYPE),
	sphereInfos(numSpheres, sphereInfos)
{
	rawSubclassPointer = this;
}

PhysicsShapeConfigMultiSphere::~PhysicsShapeConfigMultiSphere() {
	//void
}
