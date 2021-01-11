#include "PSC_BvhTriangleMeshShape.h"
#include <cassert>

using namespace physics3D;

int PSC_BvhTriangleMeshShape::TYPE = super::NewType();

PSC_BvhTriangleMeshShape::PSC_BvhTriangleMeshShape(
	float collisionMargin,
	int pointsCount, Vec3* pointsData)
	: super(TYPE),
	collisionMargin(collisionMargin),
	points(pointsCount, pointsData)
{
	rawSubclassPointer = this;

	// Make usre Vec3 has the correct size to work as an array of floats.
	// Points must be a multiple of 3 because they represent triangles.
	if (pointsCount % 3 != 0 || sizeof(Vec3) != 3 * sizeof(float)) {
		assert(false);
	}
}

void PSC_BvhTriangleMeshShape::SetQuadTriangles(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, Vec3* oPointsArray, int& pointsIndexRef) {
	oPointsArray[pointsIndexRef++] = p1;
	oPointsArray[pointsIndexRef++] = p2;
	oPointsArray[pointsIndexRef++] = p3;

	oPointsArray[pointsIndexRef++] = p1;
	oPointsArray[pointsIndexRef++] = p3;
	oPointsArray[pointsIndexRef++] = p4;
}

PSC_BvhTriangleMeshShape::~PSC_BvhTriangleMeshShape() {
	//void
}
