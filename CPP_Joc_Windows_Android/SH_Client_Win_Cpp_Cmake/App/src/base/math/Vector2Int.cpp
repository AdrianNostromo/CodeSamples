#include "Vector2Int.h"
#include <base/math/Math.h>

Vector2Int::Vector2Int()
    : x(0), y(0)
{
    //void
}

Vector2Int::Vector2Int(int x, int y)
    : x(x), y(y)
{
    //void
}

Vector2Int::Vector2Int(Vector2Int const& src)
    : x(src.x), y(src.y)
{
    //void
}

void Vector2Int::idt() {
    x = 0;
    y = 0;
}

void Vector2Int::set(int x, int y) {
    this->x = x;
    this->y = y;
}

void Vector2Int::set(Vector2Int const& src) {
    this->x = src.x;
    this->y = src.y;
}

void Vector2Int::replaceEachOther(Vector2Int& other) {
    type t_x = this->x;
    type t_y = this->y;

    this->x = other.x;
    this->y = other.y;

	other.x = t_x;
	other.y = t_y;
}

void Vector2Int::setMin(int x, int y) {
    this->x = Math::min(this->x, x);
    this->y = Math::min(this->y, y);
}

void Vector2Int::setMin(Vector2Int const& o) {
    this->x = Math::min(this->x, o.x);
    this->y = Math::min(this->y, o.y);
}

void Vector2Int::setMax(int x, int y) {
    this->x = Math::max(this->x, x);
    this->y = Math::max(this->y, y);
}

void Vector2Int::setMax(Vector2Int const& o) {
    this->x = Math::max(this->x, o.x);
    this->y = Math::max(this->y, o.y);
}

void Vector2Int::add(int x, int y) {
    this->x += x;
    this->y += y;
}

bool Vector2Int::equals(int x, int y) {
    if(this->x != x
       || this->y != y)
    {
        return false;
    }

    return true;
}

bool Vector2Int::equals(Vector2Int const& o) {
    if(this->x != o.x
       || this->y != o.y)
    {
        return false;
    }

    return true;
}

Vector2Int::~Vector2Int() {
    //void
}
