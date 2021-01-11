#include <base/math/Math.h>
#include "Bounds2DInt.h"

Bounds2DInt::Bounds2DInt()
	: min(std::numeric_limits<type>::max(), std::numeric_limits<type>::max()),
	max(std::numeric_limits<type>::min(), std::numeric_limits<type>::min())
{
	//void
}

Bounds2DInt::Bounds2DInt(Bounds2DInt const& src)
	: min(src.min), max(src.max)
{
	//void
}

Bounds2DInt::Bounds2DInt(Vector2Int min, Vector2Int max)
	: min(min), max(max)
{
	//void
}

Bounds2DInt::Bounds2DInt(
	type xMin, type xMax,
	type yMin, type yMax)
	: min(xMin, yMin), max(xMax, yMax)
{
	//void
}

void Bounds2DInt::clear() {
	min.set(std::numeric_limits<type>::max(), std::numeric_limits<type>::max());
	max.set(std::numeric_limits<type>::min(), std::numeric_limits<type>::min());
}

Bounds2DInt::type Bounds2DInt::getLenX() {
	if (min.x > max.x) {
		// No bounds are sey.
		return 0;
	}

	return max.x - min.x;
}

void Bounds2DInt::setLenX(type lenX) {
	max.x = min.x + lenX;
}

Bounds2DInt::type Bounds2DInt::getLenY() {
	if (min.y > max.y) {
		// No bounds are sey.
		return 0;
	}

	return max.y - min.y;
}

void Bounds2DInt::setLenY(type lenY) {
	max.y = min.y + lenY;
}

Bounds2DInt::type Bounds2DInt::getArea() {
	return (max.x - min.x) * (max.y - min.y);
}

bool Bounds2DInt::contains(type x, type y) {
	if (x < min.x || x >= max.x || y < min.y || y >= max.y) {
		return false;
	}

	return true;
}

bool Bounds2DInt::contains(Vector2Int& p) {
	return contains(p.x, p.y);
}

bool Bounds2DInt::contains(Vector2Int* p) {
	return contains(p->x, p->y);
}

Bounds2DInt* Bounds2DInt::set(Bounds2DInt const& src) {
	min.set(src.min);
	max.set(src.max);

	return this;
}

Bounds2DInt* Bounds2DInt::set(Vector2Int const& min, Vector2Int const& max) {
	this->min.set(min);
	this->max.set(max);

	return this;
}

Bounds2DInt* Bounds2DInt::set(
	type xMin, type xMax,
	type yMin, type yMax)
{
	min.set(xMin, yMin);
	max.set(xMax, yMax);

	return this;
}

Bounds2DInt* Bounds2DInt::unionA(
	const type xMin, const type xMax,
	const type yMin, const type yMax)
{
	min.x = Math::min(min.x, xMin);
	min.y = Math::min(min.y, yMin);

	max.x = Math::max(max.x, xMax);
	max.y = Math::max(max.y, yMax);

	return this;
}

Bounds2DInt* Bounds2DInt::intersect(Bounds2DInt& o) {
	min.setMax(o.min);
	max.setMin(o.max);

	return this;
}

Bounds2DInt* Bounds2DInt::intersect(
	const type xMin, const type xMax,
	const type yMin, const type yMax)
{
	min.setMax(xMin, yMin);
	max.setMin(xMax, yMax);

	return this;
}

Bounds2DInt* Bounds2DInt::unionOther(Bounds2DInt& o) {
	min.setMin(o.min);
	max.setMax(o.max);

	return this;
}

Bounds2DInt* Bounds2DInt::unionOther(
	const type xMin, const type xMax,
	const type yMin, const type yMax)
{
	min.setMin(xMin, yMin);
	max.setMax(xMax, yMax);

	return this;
}

bool Bounds2DInt::equals(Bounds2DInt& o) {
	if (!min.equals(o.min) || !max.equals(o.max)) {
		return false;
	}

	return true;
}

bool Bounds2DInt::equals(Bounds2DInt* o) {
	if (o == nullptr) {
		return false;
	}

	return equals(*o);
}

Bounds2DInt::~Bounds2DInt() {
	//void
}
