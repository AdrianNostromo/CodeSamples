#pragma once

#include <base/math/Vector3.h>

class Ray {
public:
    Vector3 origin{};
    Vector3 direction{};

public:
    Ray(Ray const&) = delete;
    Ray(Ray &&) = default;
    Ray& operator=(Ray const&) = delete;
    Ray& operator=(Ray &&) = default;
    explicit Ray();
    explicit Ray(Vector3 origin, Vector3 direction);

	Vector3* getEndPoint(Vector3& out, const float distance);

    ~Ray();
};
