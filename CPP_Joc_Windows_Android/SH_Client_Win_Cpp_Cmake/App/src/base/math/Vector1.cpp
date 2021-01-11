#include "Vector1.h"
#include <base/math/Math.h>

Vector1::Vector1()
    : x(0.0f)
{
    //void
}

Vector1::Vector1(type x)
    : x(x)
{
    //void
}

Vector1::Vector1(Vector1 const& src)
    : x(src.x)
{
    //void
}

Vector1* Vector1::set(type x) {
    this->x = x;

    return this;
}

Vector1* Vector1::set(Vector1 const& val) {
    this->x = val.x;

    return this;
}

Vector1::type Vector1::len() {
    return Math::abs(x);
}

Vector1* Vector1::idt() {
    x = 0.0f;

    return this;
}

Vector1* Vector1::nor() {
    type _len = len();
    if (_len != 0.0f) {
        x /= _len;
    }

    return this;
}

Vector1* Vector1::clipLength(type maxLength) {
    type _len = len();
    if (_len > maxLength) {
        type mult = maxLength / _len;

        x *= mult;
    }

    return this;
}

bool Vector1::equals(Vector1& o) {
    if(x != o.x)
    {
        return false;
    }

    return true;
}

bool Vector1::equals(type x, type y) {
    if(this->x != x)
    {
        return false;
    }

    return true;
}

Vector1::~Vector1() {
	//void
}
