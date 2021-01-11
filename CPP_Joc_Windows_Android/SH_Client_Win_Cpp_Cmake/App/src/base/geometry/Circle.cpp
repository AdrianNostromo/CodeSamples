#include "Circle.h"

Circle::Circle(float x, float y, float radius)
    :super(),
     x(x), y(y), radius(radius)
{
    //void
}

Circle::Circle()
    :Circle(0.0f, 0.0f, 0.0f)
{
    //void
}

Circle* Circle::set(float x, float y, float radius) {
    this->x = x;
    this->y = y;

    this->radius = radius;

    return this;
}

/** Checks whether or not this circle contains a given point.
 *
 * @param x X coordinate
 * @param y Y coordinate
 *
 * @return true if this circle contains the given point. */
bool Circle::contains(float x, float y) {
    x = this->x - x;
    y = this->y - y;
    return x * x + y * y <= radius * radius;
}

/** Checks whether or not this circle contains a given point.
 *
 * @param point The {@link Vector2} that contains the point coordinates.
 *
 * @return true if this circle contains this point; false otherwise. */
bool Circle::contains(Vector2& point) {
    float dx = x - point.x;
    float dy = y - point.y;
    return dx * dx + dy * dy <= radius * radius;
}

Circle::~Circle() {
    //void
}
