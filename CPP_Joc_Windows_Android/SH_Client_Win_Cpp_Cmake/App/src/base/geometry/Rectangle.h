#pragma once

#include <base/geometry/Shape2D.h>

class Rectangle : public Shape2D {
private: typedef Shape2D super;
public:
    float x;
    float y;

    float width;
    float height;

public:
    Rectangle(Rectangle const&) = delete;
    Rectangle(Rectangle &&) = default;
    Rectangle& operator=(Rectangle const&) = delete;
    Rectangle& operator=(Rectangle &&) = default;

    explicit Rectangle(float x, float y, float width, float height);
    explicit Rectangle();

    Rectangle* set(float x, float y, float width, float height);

    bool contains (Vector2& point) override;
    bool contains (float x, float y) override;

    ~Rectangle() override;
};
