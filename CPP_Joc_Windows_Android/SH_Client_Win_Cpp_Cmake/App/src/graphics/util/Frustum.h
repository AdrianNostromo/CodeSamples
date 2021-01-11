#pragma once

#include <base/math/Matrix4.h>
#include "Plane.h"

class Frustum {
public:
    /** the six clipping planes, near, far, left, right, top, bottom **/
    Plane planes[6];

protected:
    static const int planePointsArrayCount = 8 * 3;

    static Vector3 clipSpacePlanePoints[8];
    static float clipSpacePlanePointsArray[planePointsArrayCount];

    /** eight points making up the near and far clipping "rectangles". order is counter clockwise, starting at bottom left **/
    Vector3 planePoints[8];
    float planePointsArray[planePointsArrayCount];

private:
    static bool isStaticsInit;

public:
    Frustum(Frustum const&) = delete;
    Frustum(Frustum &&) = default;
    Frustum& operator=(Frustum const&) = delete;
    Frustum& operator=(Frustum &&) = default;

    explicit Frustum();

    void update(Matrix4& inverseProjectionView);
    bool sphereInFrustum(float x, float y, float z, float radius) const;

    ~Frustum();
private:
    static bool autoInitStatics();

};
