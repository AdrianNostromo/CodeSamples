#include "Vector1Int.h"
#include <base/math/Math.h>

Vector1Int::Vector1Int()
    : x(0)
{
    //void
}

Vector1Int::Vector1Int(int x)
    : x(x)
{
    //void
}

Vector1Int::Vector1Int(Vector1Int const& src)
    : x(src.x)
{
    //void
}

void Vector1Int::idt() {
    x = 0;
}

void Vector1Int::set(int x) {
    this->x = x;
}

void Vector1Int::set(Vector1Int const& src) {
    this->x = src.x;
}

void Vector1Int::replaceEachOther(Vector1Int& other) {
	type t_x = this->x;

    this->x = other.x;

	other.x = t_x;
}

void Vector1Int::setMin(int x) {
    this->x = Math::min(this->x, x);
}

void Vector1Int::setMin(Vector1Int const& o) {
    this->x = Math::min(this->x, o.x);
}

void Vector1Int::setMax(int x) {
    this->x = Math::max(this->x, x);
}

void Vector1Int::setMax(Vector1Int const& o) {
    this->x = Math::max(this->x, o.x);
}

void Vector1Int::add(int x) {
    this->x += x;
}

bool Vector1Int::equals(int x) {
    if(this->x != x) {
        return false;
    }

    return true;
}

bool Vector1Int::equals(Vector1Int const& o) {
    if(this->x != o.x) {
        return false;
    }

    return true;
}

Vector1Int::~Vector1Int() {
    //void
}
