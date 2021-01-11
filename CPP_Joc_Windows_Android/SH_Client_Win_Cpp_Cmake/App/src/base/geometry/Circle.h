#pragma once

#include <base/geometry/Shape2D.h>

class Circle : public Shape2D {
private: typedef Shape2D super;
public:
    float x;
    float y;

    float radius;

public:
    Circle(Circle const&) = delete;
    Circle(Circle &&) = default;
    Circle& operator=(Circle const&) = delete;
    Circle& operator=(Circle &&) = default;

    explicit Circle(float x, float y, float radius);
    explicit Circle();

    Circle* set(float x, float y, float radius);

    bool contains (Vector2& point) override;
    bool contains (float x, float y) override;

    ~Circle() override;
};
