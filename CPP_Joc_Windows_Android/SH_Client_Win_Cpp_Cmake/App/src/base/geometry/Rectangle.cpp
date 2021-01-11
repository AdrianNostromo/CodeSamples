#include "Rectangle.h"

Rectangle::Rectangle(float x, float y, float width, float height)
  :super(),
  x(x), y(y), width(width), height(height)
{
  //void
}

Rectangle::Rectangle()
  :Rectangle(0.0f, 0.0f, 0.0f, 0.0f)
{
  //void
}

Rectangle* Rectangle::set(float x, float y, float width, float height) {
  this->x = x;
  this->y = y;

  this->width = width;
  this->height = height;

  return this;
}

/** @param x point x coordinate
 * @param y point y coordinate
 * @return whether the point is contained in the rectangle */
bool Rectangle::contains(float x, float y) {
    return this->x <= x && this->x + this->width >= x && this->y <= y && this->y + this->height >= y;
}

/** @param point The coordinates vector
 * @return whether the point is contained in the rectangle */
bool Rectangle::contains(Vector2& point) {
    return contains(point.x, point.y);
}

Rectangle::~Rectangle() {
    //void
}
