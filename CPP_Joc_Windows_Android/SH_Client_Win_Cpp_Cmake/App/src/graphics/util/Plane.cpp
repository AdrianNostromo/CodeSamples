#include "Plane.h"

Plane::Plane()
	: normal(), d(0.0f)
{
	//void
}

/** Constructs a new plane based on the normal and distance to the origin.
	 *
	 * @param normal The plane normal
	 * @param d The distance to the origin */
Plane::Plane(Vector3& normal, float d) {
    this->normal.set(normal)->nor();
    this->d = d;
}

/** Sets the plane normal and distance to the origin based on the three given points which are considered to be on the plane.
 * The normal is calculated via a cross product between (point1-point2)x(point2-point3)
 *
 * @param point1
 * @param point2
 * @param point3 */
void Plane::set(Vector3& point1, Vector3& point2, Vector3& point3) {
    normal.set(point1)->sub(point2)->crs(point2.x-point3.x, point2.y-point3.y, point2.z-point3.z)->nor();
    d = -point1.dot(normal);
}

/** @return The normal */
Vector3* Plane::getNormal() {
    return &normal;
}

/** @return The distance to the origin */
float Plane::getD() {
    return d;
}

/** Returns on which side the given point lies relative to the plane and its normal. PlaneSide.Front refers to the side the
 * plane normal points to.
 *
 * @param point The point
 * @return The side the point lies relative to the plane */
Plane::PlaneSide Plane::testPoint(Vector3& point) {
    float dist = normal.dot(point) + d;

    if (dist == 0)
        return PlaneSide::OnPlane;
    else if (dist < 0)
        return PlaneSide::Back;
    else
        return PlaneSide::Front;
}

Plane::~Plane() {
    //void
}
