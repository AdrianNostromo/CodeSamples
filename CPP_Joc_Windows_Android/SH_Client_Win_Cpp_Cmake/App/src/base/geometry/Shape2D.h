#pragma once

#include <base/math/Vector2.h>

class Shape2D {
public:
    Shape2D();

    /** Returns whether the given point is contained within the shape. */
    virtual bool contains (Vector2& point) = 0;

    /** Returns whether a point with the given coordinates is contained within the shape. */
    virtual bool contains (float x, float y) = 0;

    virtual ~Shape2D() = default;
};
