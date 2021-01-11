#include <base/math/Math.h>
#include "BoundingBox.h"
#include <limits>

Vector3 BoundingBox::tmpVector{};

BoundingBox::BoundingBox() {
    //void
}

/** @param out The {@link Vector3} to receive the center of the bounding box.
 * @return The vector specified with the out argument. */
Vector3* BoundingBox::getCenter (Vector3& out) {
    return out.set(cnt);
}

float BoundingBox::getCenterX() {
    return cnt.x;
}

float BoundingBox::getCenterY() {
    return cnt.y;
}

float BoundingBox::getCenterZ() {
    return cnt.z;
}

/** @param out The {@link Vector3} to receive the dimensions of this bounding box on all three axis.
 * @return The vector specified with the out argument */
Vector3* BoundingBox::getSize(Vector3& out) {
    return out.set(dim);
}

float BoundingBox::getSizeX() {
    return dim.x;
}

float BoundingBox::getSizeY() {
    return dim.y;
}

float BoundingBox::getSizeZ() {
    return dim.z;
}

/** Sets the bounding box minimum and maximum vector from the given points.
 *
 * @param points The points.
 * @return This bounding box for chaining. */
BoundingBox* BoundingBox::set(Vector3* points_8) {
    this->inf();
    for(int i=0;i<8;i++) {
        Vector3* l_point = &points_8[i];

        this->ext(*l_point);
    }

    return this;
}

/** Sets the given minimum and maximum vector.
 *
 * @param minimum The minimum vector
 * @param maximum The maximum vector
 * @return This bounding box for chaining. */
BoundingBox* BoundingBox::set(Vector3& minimum, Vector3& maximum) {
    min.set(minimum.x < maximum.x ? minimum.x : maximum.x, minimum.y < maximum.y ? minimum.y : maximum.y,
            minimum.z < maximum.z ? minimum.z : maximum.z);
    max.set(minimum.x > maximum.x ? minimum.x : maximum.x, minimum.y > maximum.y ? minimum.y : maximum.y,
            minimum.z > maximum.z ? minimum.z : maximum.z);
    cnt.set(min)->add(max)->scl(0.5f);
    dim.set(max)->sub(min);
    return this;
}

/** Sets the minimum and maximum vector to positive and negative infinity.
 *
 * @return This bounding box for chaining. */
BoundingBox* BoundingBox::inf() {
    min.set(
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity()
    );
    max.set(
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity()
    );
    cnt.set(0, 0, 0);
    dim.set(0, 0, 0);
    return this;
}

/** Extends the bounding box to incorporate the given {@link Vector3}.
 * @param point The vector
 * @return This bounding box for chaining. */
BoundingBox* BoundingBox::ext(Vector3& point) {
    return this->set(
        *(min.set(Math::min(min.x, point.x), Math::min(min.y, point.y), Math::min(min.z, point.z))),
        *(max.set(Math::max(max.x, point.x), Math::max(max.y, point.y), Math::max(max.z, point.z)))
    );
}

BoundingBox::~BoundingBox() {
    //void
}
