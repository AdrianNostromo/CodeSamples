#pragma once

#include <base/gh.h>
#include "Ray.h"
#include "BoundingBox.h"
#include <graphics/util/Plane.h>
#include <base/math/Vector2.h>
#include <base/math/Vector3.h>
#include <base/list/Array1D.h>

class Intersector {
private:
    static Vector3 best;
    static Vector3 tmp;
    static Vector3 tmp1;
    static Vector3 tmp2;
    static Vector3 tmp3;
    static Vector2 v2tmp;

    static Vector3 v0;
    static Vector3 v1;
    static Vector3 v2;

public:
    static bool intersectRayBoundsFast (Ray& ray, BoundingBox& box, Vector3* intersectionPoint);
    static bool intersectRayBoundsFast (Ray& ray, Vector3& center, Vector3& dimensions, Vector3* intersectionPoint);

    static bool intersectRayPlane (Ray& ray, Plane& plane, Vector3* intersection);

	pub static bool isPointInTriangle(Vector3& point, Vector3& t1, Vector3& t2, Vector3& t3);
	pub static bool isPointInTriangle(Vector2& p, Vector2& a, Vector2& b, Vector2& c);
	pub static bool isPointInTriangle(float px, float py, float ax, float ay, float bx, float by, float cx, float cy);

	pub static bool intersectRayTriangle(Ray& ray, Vector3& t1, Vector3& t2, Vector3& t3, Vector3* intersection);

	pub static bool intersectRayTriangles(Ray& ray, Array1D<float>& triangles, Vector3* intersection);

};
