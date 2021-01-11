#pragma once

#include <base/math/Vector3.h>

class Plane {
public:
    enum PlaneSide {
        OnPlane, Back, Front
    };

public:
    Vector3 normal;
    float d;

public:
    Plane(Plane const&) = delete;
    Plane(Plane &&) = default;
    Plane& operator=(Plane const&) = delete;
    Plane& operator=(Plane &&) = default;
    
    explicit Plane();
    explicit Plane(Vector3& normal, float d);

    void set(Vector3& point1, Vector3& point2, Vector3& point3);

    Vector3* getNormal();
    float getD();

    PlaneSide testPoint(Vector3& point);

    ~Plane();
};
