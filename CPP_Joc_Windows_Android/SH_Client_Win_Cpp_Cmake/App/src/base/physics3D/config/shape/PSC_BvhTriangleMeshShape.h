#pragma once

#include <base/gh.h>
#include "PhysicsShapeConfig.h"
#include <base/list/Array1D.h>
#include <base/math/Vector3.h>

namespace physics3D {
class PSC_BvhTriangleMeshShape : public PhysicsShapeConfig {priv typedef PhysicsShapeConfig super;pub dCtor(PSC_BvhTriangleMeshShape);
	pub class Vec3 {
		pub float x;
		pub float y;
		pub float z;
	};
	
	pub static int TYPE;

	pub float collisionMargin;
	// Each 3 points form a triangle.
	// The total number of points must be a multiple of 3;
	pub Array1D<Vec3> points;
    
	pub explicit PSC_BvhTriangleMeshShape(
		float collisionMargin,
		int pointsCount, Vec3* pointsData);

	pub static void SetQuadTriangles(Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, Vec3* oPointsArray, int& pointsIndexRef);

    pub ~PSC_BvhTriangleMeshShape() override;
};
};
