#include <base/math/Math.h>
#include "Bounds1DInt.h"

Bounds1DInt::Bounds1DInt()
	: min(std::numeric_limits<type>::max()),
	max(std::numeric_limits<type>::min())
{
	//void
}

Bounds1DInt::Bounds1DInt(Bounds1DInt const& src)
	: min(src.min), max(src.max)
{
	//void
}

Bounds1DInt::Bounds1DInt(Vector1Int min, Vector1Int max)
	: min(min), max(max)
{
	//void
}

Bounds1DInt::Bounds1DInt(type xMin, type xMax)
	: min(xMin), max(xMax)
{
	//void
}

void Bounds1DInt::clear() {
	min.set(std::numeric_limits<type>::max());
	max.set(std::numeric_limits<type>::min());
}

Bounds1DInt::type Bounds1DInt::getLenX() {
	if (min.x > max.x) {
		// No bounds are sey.
		return 0;
	}

	return max.x - min.x;
}

void Bounds1DInt::setLenX(type lenX) {
	max.x = min.x + lenX;
}

bool Bounds1DInt::contains(type x) {
	if (x < min.x || x >= max.x) {
		return false;
	}

	return true;
}

bool Bounds1DInt::contains(Vector1Int& p) {
	return contains(p.x);
}

bool Bounds1DInt::contains(Vector1Int* p) {
	return contains(p->x);
}

Bounds1DInt* Bounds1DInt::set(Bounds1DInt const& src) {
	min.set(src.min);
	max.set(src.max);

	return this;
}

Bounds1DInt* Bounds1DInt::set(Vector1Int const& min, Vector1Int const& max) {
	this->min.set(min);
	this->max.set(max);

	return this;
}

Bounds1DInt* Bounds1DInt::set(type xMin, type xMax) {
	min.set(xMin);
	max.set(xMax);

	return this;
}

Bounds1DInt* Bounds1DInt::intersect(Bounds1DInt& o) {
	min.setMax(o.min);
	max.setMin(o.max);

	return this;
}

Bounds1DInt* Bounds1DInt::intersect(const type xMin, const type xMax) {
	min.setMax(xMin);
	max.setMin(xMax);

	return this;
}

Bounds1DInt* Bounds1DInt::unionOther(Bounds1DInt& o) {
	min.setMin(o.min);
	max.setMax(o.max);

	return this;
}

Bounds1DInt* Bounds1DInt::unionOther(const type xMin, const type xMax) {
	min.setMin(xMin);
	max.setMax(xMax);

	return this;
}

bool Bounds1DInt::equals(Bounds1DInt& o) {
	if (!min.equals(o.min) || !max.equals(o.max)) {
		return false;
	}

	return true;
}

bool Bounds1DInt::equals(Bounds1DInt* o) {
	if (o == nullptr) {
		return false;
	}

	return equals(*o);
}

Bounds1DInt::~Bounds1DInt() {
	//void
}
