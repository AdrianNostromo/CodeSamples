#include "Vector3Int.h"
#include <base/math/Math.h>

Vector3Int::Vector3Int()
    :Vector3Int(0, 0, 0)
{
	//void
}

Vector3Int::Vector3Int(type x, type y, type z) {
	set(x, y, z);
}

Vector3Int::Vector3Int(Vector3Int const& o) {
	set(o.x, o.y, o.z);
}

Vector3Int::Vector3Int(Vector3Int const* o) {
	set(o->x, o->y, o->z);
}

void Vector3Int::set(Vector3Int const& src) {
	this->x = src.x;
	this->y = src.y;
	this->z = src.z;
}

void Vector3Int::set(type x, type y, type z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3Int::add(Vector3Int const& src) {
	this->x += src.x;
	this->y += src.y;
	this->z += src.z;
}

void Vector3Int::add(type x, type y, type z) {
	this->x += x;
	this->y += y;
	this->z += z;
}

void Vector3Int::min(type x, type y, type z) {
	setMin(x, y, z);
}

void Vector3Int::min(Vector3Int const& o) {
	setMin(o);
}

void Vector3Int::max(type x, type y, type z) {
	setMax(x, y, z);
}

void Vector3Int::max(Vector3Int const& o) {
	setMax(o);
}

void Vector3Int::setMin(type x, type y, type z) {
	this->x = Math::min(this->x, x);
	this->y = Math::min(this->y, y);
	this->z = Math::min(this->z, z);
}

void Vector3Int::setMin(Vector3Int const& o) {
	this->x = Math::min(this->x, o.x);
	this->y = Math::min(this->y, o.y);
	this->z = Math::min(this->z, o.z);
}

void Vector3Int::setMax(type x, type y, type z) {
	this->x = Math::max(this->x, x);
	this->y = Math::max(this->y, y);
	this->z = Math::max(this->z, z);
}

void Vector3Int::setMax(Vector3Int const& o) {
	this->x = Math::max(this->x, o.x);
	this->y = Math::max(this->y, o.y);
	this->z = Math::max(this->z, o.z);
}

bool Vector3Int::equals(Vector3Int const& o) {
	if (this->x != o.x || this->y != o.y || this->z != o.z) {
		return false;
	}

	return true;
}

bool Vector3Int::equals(type x, type y, type z) {
	if (this->x != x || this->y != y || this->z != z) {
		return false;
	}

	return true;
}

Vector3Int::~Vector3Int() {
	//void
}
