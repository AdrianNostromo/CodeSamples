#include "Ray.h"

Ray::Ray() {
    //void
}

/** Constructor, sets the starting position of the ray and the direction.
 *
 * @param origin The starting position
 * @param direction The direction */
Ray::Ray(Vector3 origin, Vector3 direction) {
    this->origin.set(origin);
    this->direction.set(direction)->nor();
}

/** Returns the endpoint given the distance. This is calculated as startpoint + distance * direction.
 * @param out The vector to set to the result
 * @param distance The distance from the end point to the start point.
 * @return The out param */
Vector3* Ray::getEndPoint(Vector3& out, const float distance) {
	return out.set(direction)->scl(distance)->add(origin);
}

Ray::~Ray() {
    //void
}
