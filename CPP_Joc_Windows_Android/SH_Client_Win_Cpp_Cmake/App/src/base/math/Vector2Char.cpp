#include "Vector2Char.h"
#include <base/math/Math.h>

Vector2Char::Vector2Char()
    :Vector2Char(0, 0)
{
    //void
}

Vector2Char::Vector2Char(type x, type y)
{
    set(x, y);
}

Vector2Char::Vector2Char(Vector2Char const& src)
    : x(src.x), y(src.y)
{
    //void
}

void Vector2Char::idt() {
    x = 0;
    y = 0;
}

void Vector2Char::set(type x, type y) {
    this->x = x;
    this->y = y;
}

void Vector2Char::set(Vector2Char const& src) {
    this->x = src.x;
    this->y = src.y;
}

void Vector2Char::replaceEachOther(Vector2Char& other) {
	type t_x = this->x;
    type t_y = this->y;

    this->x = other.x;
    this->y = other.y;

	other.x = t_x;
	other.y = t_y;
}

void Vector2Char::setMin(type x, type y) {
    this->x = static_cast<char>(Math::min(this->x, x));
    this->y = static_cast<char>(Math::min(this->y, y));
}

void Vector2Char::setMin(Vector2Char const& o) {
    this->x = static_cast<char>(Math::min(this->x, o.x));
    this->y = static_cast<char>(Math::min(this->y, o.y));
}

void Vector2Char::setMax(type x, type y) {
    this->x = static_cast<char>(Math::max(this->x, x));
    this->y = static_cast<char>(Math::max(this->y, y));
}

void Vector2Char::setMax(Vector2Char const& o) {
    this->x = static_cast<char>(Math::max(this->x, o.x));
    this->y = static_cast<char>(Math::max(this->y, o.y));
}

void Vector2Char::add(type x, type y) {
    this->x += x;
    this->y += y;
}

bool Vector2Char::equals(type x, type y) {
    if(this->x != x
       || this->y != y)
    {
        return false;
    }

    return true;
}

bool Vector2Char::equals(Vector2Char const& o) {
    if(this->x != o.x
       || this->y != o.y)
    {
        return false;
    }

    return true;
}

Vector2Char::~Vector2Char() {
    //void
}
