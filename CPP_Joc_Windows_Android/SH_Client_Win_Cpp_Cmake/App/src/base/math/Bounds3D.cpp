#include <base/math/Math.h>
#include "Bounds3D.h"

Bounds3D::Bounds3D()
    : min(std::numeric_limits<type>::max(), std::numeric_limits<type>::max(), std::numeric_limits<type>::max()),
    max(std::numeric_limits<type>::min(), std::numeric_limits<type>::min(), std::numeric_limits<type>::min())
{
    //void
}

Bounds3D::Bounds3D(Bounds3D const& src)
    : min(src.min), max(src.max)
{
    //void
}

Bounds3D::Bounds3D(Vector3 min, Vector3 max)
    : min(min), max(max)
{
    //void
}

void Bounds3D::clear() {
    min.set(std::numeric_limits<type>::max(), std::numeric_limits<type>::max(), std::numeric_limits<type>::max());
    max.set(std::numeric_limits<type>::min(), std::numeric_limits<type>::min(), std::numeric_limits<type>::min());
}

Bounds3D::type Bounds3D::getLenX() {
    if (min.x > max.x) {
        // No bounds are sey.
        return 0;
    }

    return max.x - min.x;
}

Bounds3D::type Bounds3D::getLenY() {
    if (min.y > max.y) {
        // No bounds are sey.
        return 0;
    }

    return max.y - min.y;
}

Bounds3D::type Bounds3D::getLenZ() {
    if (min.z > max.z) {
        // No bounds are sey.
        return 0;
    }

    return max.z - min.z;
}

bool Bounds3D::containsEdgeInclusive(type x, type y, type z) {
    if(x < min.x || x > max.x || y < min.y || y > max.y || z < min.z || z > max.z) {
        return false;
    }

    return true;
}

bool Bounds3D::containsEdgeInclusive(Vector3& p) {
    return containsEdgeInclusive(p.x, p.y, p.z);
}

bool Bounds3D::containsEdgeInclusive(Vector3* p) {
    return containsEdgeInclusive(p->x, p->y, p->z);
}

void Bounds3D::set(Bounds3D const& src) {
    min.set(src.min);
    max.set(src.max);
}

void Bounds3D::set(Vector3 const& min, Vector3 const& max) {
    this->min.set(min);
    this->max.set(max);
}

Bounds3D* Bounds3D::unionPoint(
    const type x,
    const type y,
    const type z
) {
    min.x = Math::min(min.x, x);
    min.y = Math::min(min.y, y);
    min.z = Math::min(min.z, z);

    max.x = Math::max(max.x, x);
    max.y = Math::max(max.y, y);
    max.z = Math::max(max.z, z);

    return this;
}

Bounds3D::~Bounds3D() {
    //void
}
