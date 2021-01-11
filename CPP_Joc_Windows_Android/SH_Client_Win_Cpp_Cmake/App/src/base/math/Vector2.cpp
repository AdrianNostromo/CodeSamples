#include <base/math/Math.h>
#include "Vector2.h"

Vector2::Vector2()
    : x(0.0f), y(0.0f)
{
  //void
}

Vector2::Vector2(type x, type y)
    : x(x), y(y)
{
  //void
}

Vector2::Vector2(Vector2 const& src)
    : x(src.x), y(src.y)
{
  //void
}

Vector2* Vector2::set(type x, type y) {
    this->x = x;
    this->y = y;

    return this;
}

Vector2* Vector2::set(Vector2 const& val) {
    this->x = val.x;
    this->y = val.y;

    return this;
}

Vector2* Vector2::add(type x, type y) {
    this->x += x;
    this->y += y;

    return this;
}

Vector2* Vector2::add(Vector2 const& val) {
    this->x += val.x;
    this->y += val.y;

    return this;
}

Vector2* Vector2::add(Vector2 const* val) {
    this->x += val->x;
    this->y += val->y;

    return this;
}

Vector2* Vector2::mul(type val) {
    this->x *= val;
    this->y *= val;

    return this;
}

Vector2::type Vector2::len() {
    return Math::sqrt(x * x + y * y);
}

Vector2* Vector2::nor() {
    type _len = len();
    if (_len != 0.0f) {
        x /= _len;
        y /= _len;
    }

    return this;
}

Vector2* Vector2::idt() {
    x = 0.0f;
    y = 0.0f;

    return this;
}

Vector2* Vector2::clipLength(type maxLength) {
    type _len = len();
    if (_len > maxLength) {
    	type mult = maxLength / _len;

        x *= mult;
        y *= mult;
    }

    return this;
}

bool Vector2::equals(Vector2& o) {
    if(x != o.x
       || y != o.y)
    {
        return false;
    }

    return true;
}

bool Vector2::equals(type x, type y) {
    if(this->x != x
       || this->y != y)
    {
        return false;
    }

    return true;
}

Vector2::~Vector2() {
    //void
}
