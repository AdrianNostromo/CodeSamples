#include "PhysicsShapeConfigConvexHull.h"
#include <cassert>

using namespace physics3D;

int PhysicsShapeConfigConvexHull::TYPE = super::NewType();

PhysicsShapeConfigConvexHull::PhysicsShapeConfigConvexHull(
	float collisionMargin,
	int pointsCount, Vec3* pointsData)
	: super(TYPE),
	collisionMargin(collisionMargin),
	points(pointsCount, pointsData)
{
	rawSubclassPointer = this;

	// Make usre Vec3 has the correct size to work as an array of floats.
	if (sizeof(Vec3) != 3 * sizeof(float)) {
		assert(false);
	}
}

PhysicsShapeConfigConvexHull::~PhysicsShapeConfigConvexHull() {
	//void
}
