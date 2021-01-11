#pragma once

#include <base/math/Vector3.h>

class BoundingBox {
public:
    Vector3 min{};
    Vector3 max{};

private:
    static Vector3 tmpVector;

    Vector3 cnt{};
    Vector3 dim{};

public:
    BoundingBox();
    Vector3* getCenter (Vector3& out);
    float getCenterX();
    float getCenterY();
    float getCenterZ();
    Vector3* getSize(Vector3& out);
    float getSizeX();
    float getSizeY();
    float getSizeZ();
    BoundingBox* set(Vector3* points_8);
    BoundingBox* set(Vector3& minimum, Vector3& maximum);
    BoundingBox* inf();
    BoundingBox* ext(Vector3& point);
    ~BoundingBox();
};
