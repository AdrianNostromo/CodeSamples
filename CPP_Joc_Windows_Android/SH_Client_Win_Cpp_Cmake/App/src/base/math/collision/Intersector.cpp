#include <base/math/Math.h>
#include "Intersector.h"
#include <memory>
#include <limits>
#include <graphics/util/Plane.h>

Vector3 Intersector::best{};
Vector3 Intersector::tmp{};
Vector3 Intersector::tmp1{};
Vector3 Intersector::tmp2{};
Vector3 Intersector::tmp3{};
Vector2 Intersector::v2tmp{};

Vector3 Intersector::v0{};
Vector3 Intersector::v1{};
Vector3 Intersector::v2{};

/** Quick check whether the given {@link Ray} and {@link BoundingBox} intersect.
 *
 * @param ray The ray
 * @param box The bounding box
 * @return Whether the ray and the bounding box intersect. */
bool Intersector::intersectRayBoundsFast (Ray& ray, BoundingBox& box, Vector3* intersectionPoint) {
    return intersectRayBoundsFast(ray, *box.getCenter(tmp1), *box.getSize(tmp2), intersectionPoint);
}

/** Quick check whether the given {@link Ray} and {@link BoundingBox} intersect.
 *
 * @param ray The ray
 * @param center The center of the bounding box
 * @param dimensions The dimensions (width, height and depth) of the bounding box
 * @return Whether the ray and the bounding box intersect. */
bool Intersector::intersectRayBoundsFast (Ray& ray, Vector3& center, Vector3& dimensions, Vector3* intersectionPoint) {
    float divX = 1.0f / ray.direction.x;
    float divY = 1.0f / ray.direction.y;
    float divZ = 1.0f / ray.direction.z;

    float minx = ((center.x - dimensions.x * .5f) - ray.origin.x) * divX;
    float maxx = ((center.x + dimensions.x * .5f) - ray.origin.x) * divX;
    if (minx > maxx) {
        float t = minx;
        minx = maxx;
        maxx = t;
    }

    float miny = ((center.y - dimensions.y * .5f) - ray.origin.y) * divY;
    float maxy = ((center.y + dimensions.y * .5f) - ray.origin.y) * divY;
    if (miny > maxy) {
        float t = miny;
        miny = maxy;
        maxy = t;
    }

    float minz = ((center.z - dimensions.z * .5f) - ray.origin.z) * divZ;
    float maxz = ((center.z + dimensions.z * .5f) - ray.origin.z) * divZ;
    if (minz > maxz) {
        float t = minz;
        minz = maxz;
        maxz = t;
    }

    float min = Math::max(Math::max(minx, miny), minz);
    float max = Math::min(Math::min(maxx, maxy), maxz);

    intersectionPoint->set(ray.direction)->scl(min)->add(ray.origin);

    return max >= 0 && max >= min;
}

/** Intersects a {@link Ray} and a {@link Plane}. The intersection point is stored in intersection in case an intersection is
 * present.
 *
 * @param ray The ray
 * @param plane The plane
 * @param intersection The vector the intersection point is written to (optional)
 * @return Whether an intersection is present. */
bool Intersector::intersectRayPlane (Ray& ray, Plane& plane, Vector3* intersection) {
    float denom = ray.direction.dot(*plane.getNormal());
    if (denom != 0) {
        float t = -(ray.origin.dot(*plane.getNormal()) + plane.getD()) / denom;
        if (t < 0) return false;

        if (intersection != nullptr) {
            intersection->set(ray.origin)->add(*v0.set(ray.direction)->scl(t));
        }
        return true;
    } else if (plane.testPoint(ray.origin) == Plane::PlaneSide::OnPlane) {
        if (intersection != nullptr) {
            intersection->set(ray.origin);
        }
        return true;
    } else
        return false;
}

/** Returns whether the given point is inside the triangle. This assumes that the point is on the plane of the triangle. No
 * check is performed that this is the case.
 *
 * @param point the point
 * @param t1 the first vertex of the triangle
 * @param t2 the second vertex of the triangle
 * @param t3 the third vertex of the triangle
 * @return whether the point is in the triangle */
bool Intersector::isPointInTriangle(Vector3& point, Vector3& t1, Vector3& t2, Vector3& t3) {
	v0.set(t1)->sub(point);
	v1.set(t2)->sub(point);
	v2.set(t3)->sub(point);

	float ab = v0.dot(v1);
	float ac = v0.dot(v2);
	float bc = v1.dot(v2);
	float cc = v2.dot(v2);

	if (bc * ac - cc * ab < 0) return false;
	float bb = v1.dot(v1);
	if (ab* bc - ac * bb < 0) return false;
	return true;
}

/** Returns true if the given point is inside the triangle. */
bool Intersector::isPointInTriangle(Vector2& p, Vector2& a, Vector2& b, Vector2& c) {
	float px1 = p.x - a.x;
	float py1 = p.y - a.y;
	bool side12 = (b.x - a.x) * py1 - (b.y - a.y) * px1 > 0;
	if ((c.x - a.x) * py1 - (c.y - a.y) * px1 > 0 == side12) return false;
	if ((c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x) > 0 != side12) return false;
	return true;
}

/** Returns true if the given point is inside the triangle. */
bool Intersector::isPointInTriangle(float px, float py, float ax, float ay, float bx, float by, float cx, float cy) {
	float px1 = px - ax;
	float py1 = py - ay;
	bool side12 = (bx - ax) * py1 - (by - ay) * px1 > 0;
	if ((cx - ax) * py1 - (cy - ay) * px1 > 0 == side12) return false;
	if ((cx - bx) * (py - by) - (cy - by) * (px - bx) > 0 != side12) return false;
	return true;
}

/** Intersect a {@link Ray} and a triangle, returning the intersection point in intersection.
 *
 * @param ray The ray
 * @param t1 The first vertex of the triangle
 * @param t2 The second vertex of the triangle
 * @param t3 The third vertex of the triangle
 * @param intersection The intersection point (optional)
 * @return True in case an intersection is present. */
bool Intersector::intersectRayTriangle(Ray& ray, Vector3& t1, Vector3& t2, Vector3& t3, Vector3* intersection) {
	Vector3 edge1{ v0.set(t2)->sub(t1) };
	Vector3 edge2{ v1.set(t3)->sub(t1) };

	Vector3 pvec{ v2.set(ray.direction)->crs(edge2) };
	float det = edge1.dot(pvec);
	if (Math::isZero(det)) {
		static Plane p{};
		p.set(t1, t2, t3);
		if (p.testPoint(ray.origin) == Plane::PlaneSide::OnPlane && Intersector::isPointInTriangle(ray.origin, t1, t2, t3)) {
			if (intersection != nullptr) {
				intersection->set(ray.origin);
			}

			return true;
		}
		return false;
	}

	det = 1.0f / det;

	static Vector3 i{};
	Vector3 tvec{ i.set(ray.origin)->sub(t1) };
	float u = tvec.dot(pvec) * det;
	if (u < 0.0f || u > 1.0f) return false;

	Vector3 qvec{ tvec.crs(edge1) };
	float v = ray.direction.dot(qvec) * det;
	if (v < 0.0f || u + v > 1.0f) return false;

	float t = edge2.dot(qvec) * det;
	if (t < 0) return false;

	if (intersection != nullptr) {
		if (t <= Math::FLOAT_ROUNDING_ERROR) {
			intersection->set(ray.origin);
		} else {
			ray.getEndPoint(*intersection, t);
		}
	}

	return true;
}

/** Intersects the given ray with list of triangles. Returns the nearest intersection point in intersection
 *
 * @param ray The ray
 * @param triangles The triangles, each successive 9 elements are the 3 vertices of a triangle, a vertex is made of 3
 *           successive floats (XYZ)
 * @param intersection The nearest intersection point (optional)
 * @return Whether the ray and the triangles intersect. */
bool Intersector::intersectRayTriangles(Ray& ray, Array1D<float>& triangles, Vector3* intersection) {
	float min_dist = std::numeric_limits<float>::max();
	bool hit = false;

	if (triangles.getCount() % 9 != 0) {
		//"triangles array size is not a multiple of 9";
		throw LogicException(LOC);
	}

	for (int i = 0; i < triangles.getCount(); i += 9) {
		bool result = intersectRayTriangle(
			ray, 
			*tmp1.set(triangles.getDirect(i), triangles.getDirect(i + 1), triangles.getDirect(i + 2)),
			*tmp2.set(triangles.getDirect(i + 3), triangles.getDirect(i + 4), triangles.getDirect(i + 5)),
			*tmp3.set(triangles.getDirect(i + 6), triangles.getDirect(i + 7), triangles.getDirect(i + 8)),
			&tmp
		);

		if (result) {
			float dist = ray.origin.dst2(tmp);
			if (dist < min_dist) {
				min_dist = dist;
				best.set(tmp);
				hit = true;
			}
		}
	}

	if (!hit)
		return false;
	else {
		if (intersection != nullptr) {
			intersection->set(best);
		}

		return true;
	}
}
