#include <base/math/Math.h>
#include "Bounds3DInt.h"

Bounds3DInt::Bounds3DInt()
	: min(std::numeric_limits<type>::max(), std::numeric_limits<type>::max(), std::numeric_limits<type>::max()),
	max(std::numeric_limits<type>::min(), std::numeric_limits<type>::min(), std::numeric_limits<type>::min())
{
	//void
}

Bounds3DInt::Bounds3DInt(Bounds3DInt const& src)
	: min(src.min), max(src.max)
{
	//void
}

Bounds3DInt::Bounds3DInt(Vector3Int min, Vector3Int max)
	: min(min), max(max)
{
	//void
}

Bounds3DInt::Bounds3DInt(
	type xMin, type xMax,
	type yMin, type yMax,
	type zMin, type zMax)
	: min(xMin, yMin, zMin), max(xMax, yMax, zMax)

{
	//void
}

void Bounds3DInt::clear() {
	min.set(std::numeric_limits<type>::max(), std::numeric_limits<type>::max(), std::numeric_limits<type>::max());
	max.set(std::numeric_limits<type>::min(), std::numeric_limits<type>::min(), std::numeric_limits<type>::min());
}

Bounds3DInt::type Bounds3DInt::getLenX() {
	if (min.x > max.x) {
		// No bounds are sey.
		return 0;
	}

	return max.x - min.x;
}

Bounds3DInt::type Bounds3DInt::getLenY() {
	if (min.y > max.y) {
		// No bounds are sey.
		return 0;
	}

	return max.y - min.y;
}

Bounds3DInt::type Bounds3DInt::getLenZ() {
	if (min.z > max.z) {
		// No bounds are sey.
		return 0;
	}

	return max.z - min.z;
}

bool Bounds3DInt::containsEdgeInclusive(type x, type y, type z) {
	if (x < min.x || x > max.x || y < min.y || y > max.y || z < min.z || z > max.z) {
		return false;
	}

	return true;
}

bool Bounds3DInt::containsEdgeInclusive(Vector3Int& p) {
	return containsEdgeInclusive(p.x, p.y, p.z);
}

bool Bounds3DInt::containsEdgeInclusive(Vector3Int* p) {
	return containsEdgeInclusive(p->x, p->y, p->z);
}

Bounds3DInt* Bounds3DInt::set(Bounds3DInt const& src) {
	min.set(src.min);
	max.set(src.max);

	return this;
}

Bounds3DInt* Bounds3DInt::set(Vector3Int const& min, Vector3Int const& max) {
	this->min.set(min);
	this->max.set(max);

	return this;
}

Bounds3DInt* Bounds3DInt::set(
	type xMin, type xMax,
	type yMin, type yMax,
	type zMin, type zMax)
{
	min.set(xMin, yMin, zMin);
	max.set(xMax, yMax, zMax);

	return this;
}

Bounds3DInt* Bounds3DInt::unionA(
	const type xMin, const type xMax,
	const type yMin, const type yMax,
	const type zMin, const type zMax)
{
	min.x = Math::min(min.x, xMin);
	min.y = Math::min(min.y, yMin);
	min.z = Math::min(min.z, zMin);

	max.x = Math::max(max.x, xMax);
	max.y = Math::max(max.y, yMax);
	max.z = Math::max(max.z, zMax);

	return this;
}

Bounds3DInt* Bounds3DInt::intersect(Bounds3DInt& o) {
	min.max(o.min);
	max.min(o.max);

	return this;
}

bool Bounds3DInt::equals(Bounds3DInt& o) {
	if (!min.equals(o.min) || !max.equals(o.max)) {
		return false;
	}

	return true;
}

bool Bounds3DInt::equals(Bounds3DInt* o) {
	if (o == nullptr) {
		return false;
	}

	return equals(*o);
}

Bounds3DInt::~Bounds3DInt() {
	//void
}
