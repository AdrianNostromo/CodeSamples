#include <base/math/Math.h>
#include "Bounds2D.h"
#include <limits>

Bounds2D::Bounds2D()
    : min(std::numeric_limits<type>::max(), std::numeric_limits<type>::max()),
    max(std::numeric_limits<type>::min(), std::numeric_limits<type>::min())
{
    //void
}

Bounds2D::Bounds2D(type xMin, type yMin, type xMax, type yMax)
    : min(xMin, yMin), max(xMax, yMax)
{
    //void
}

Bounds2D::Bounds2D(Bounds2D const& src)
    : min(src.min), max(src.max)
{
    //void
}

void Bounds2D::clear() {
    min.set(std::numeric_limits<type>::max(), std::numeric_limits<type>::max());
    max.set(std::numeric_limits<type>::min(), std::numeric_limits<type>::min());
}

Bounds2D::type Bounds2D::getWidth() {
    if (min.x > max.x) {
        // No bounds are sey.
        return 0;
    }

    return max.x - min.x;
}

Bounds2D::type Bounds2D::getHeight() {
    if (min.y > max.y) {
        // No bounds are sey.
        return 0;
    }

    return max.y - min.y;
}

bool Bounds2D::contains(type x, type y) {
    if(x < min.x || x >= max.x || y < min.y || y >= max.y) {
        return false;
    }
    
    return true;
}

bool Bounds2D::containsEdgeInclusive(type x, type y) {
    if(x < min.x || x > max.x || y < min.y || y > max.y) {
        return false;
    }
    
    return true;
}

void Bounds2D::set(type xMin, type yMin, type xMax, type yMax) {
    this->min.x = xMin;
    this->min.y = yMin;

    this->max.x = xMax;
    this->max.y = yMax;
}

void Bounds2D::set(Bounds2D& src) {
    set(src.min.x, src.min.y, src.max.x, src.max.y);
}

Bounds2D* Bounds2D::unionPoint(
    const type x,
    const type y
) {
    min.x = Math::min(min.x, x);
    min.y = Math::min(min.y, y);

    max.x = Math::max(max.x, x);
    max.y = Math::max(max.y, y);
    
    return this;
}

Bounds2D* Bounds2D::unionBounds(
	const type xMin, const type yMin,
	const type xMax, const type yMax)
{
	this->min.x = Math::min(this->min.x, xMin);
	this->min.y = Math::min(this->min.y, yMin);

	this->max.x = Math::max(this->max.x, xMax);
	this->max.y = Math::max(this->max.y, yMax);

	return this;
}

bool Bounds2D::equals(Bounds2D& other) {
    if (!this->min.equals(other.min) || !this->max.equals(other.max)) {
        return false;
    }

    return true;
}

bool Bounds2D::equals(Bounds2D* other) {
    if (other == nullptr || !this->min.equals(other->min) || !this->max.equals(other->max)) {
        return false;
    }

    return true;
}

Bounds2D::~Bounds2D() {
    //void
}
