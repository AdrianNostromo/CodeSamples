#include "Frustum.h"
#include <cstring>

Vector3 Frustum::clipSpacePlanePoints[8] = {Vector3(-1, -1, -1), Vector3(1, -1, -1),
    Vector3(1, 1, -1), Vector3(-1, 1, -1), // near clip
    Vector3(-1, -1, 1), Vector3(1, -1, 1), Vector3(1, 1, 1), Vector3(-1, 1, 1)}; // far clip
float Frustum::clipSpacePlanePointsArray[8 * 3];

bool Frustum::isStaticsInit = autoInitStatics();

bool Frustum::autoInitStatics() {
    int j = 0;
    for(int i=0;i<8;i++) {
        Vector3* v = &clipSpacePlanePoints[i];

        clipSpacePlanePointsArray[j++] = v->x;
        clipSpacePlanePointsArray[j++] = v->y;
        clipSpacePlanePointsArray[j++] = v->z;
    }

    return true;
}

Frustum::Frustum()
    : planes
    {
        Plane(),
        Plane(),
        Plane(),
        Plane(),
        Plane(),
        Plane()
    }
{
    //void
}

/** Updates the clipping plane's based on the given inverse combined projection and view matrix, e.g. from an
 * {@link OrthographicCamera} or {@link PerspectiveCamera}.
 * @param inverseProjectionView the combined projection and view matrices. */
void Frustum::update(Matrix4& inverseProjectionView) {
    memcpy(
            planePointsArray,
            clipSpacePlanePointsArray,
            planePointsArrayCount * sizeof(float)
    );

    Matrix4::prj(inverseProjectionView.val, planePointsArray, 0, 8, 3);
    for (int i = 0, j = 0; i < 8; i++) {
        Vector3* v = &planePoints[i];
        v->x = planePointsArray[j++];
        v->y = planePointsArray[j++];
        v->z = planePointsArray[j++];
    }

    planes[0].set(planePoints[1], planePoints[0], planePoints[2]);
    planes[1].set(planePoints[4], planePoints[5], planePoints[7]);
    planes[2].set(planePoints[0], planePoints[4], planePoints[3]);
    planes[3].set(planePoints[5], planePoints[1], planePoints[6]);
    planes[4].set(planePoints[2], planePoints[3], planePoints[6]);
    planes[5].set(planePoints[4], planePoints[0], planePoints[1]);
}

/** Returns whether the given sphere is in the frustum.
 *
 * @param x The X coordinate of the center of the sphere
 * @param y The Y coordinate of the center of the sphere
 * @param z The Z coordinate of the center of the sphere
 * @param radius The radius of the sphere
 * @return Whether the sphere is in the frustum */
bool Frustum::sphereInFrustum(float x, float y, float z, float radius) const {
    for (int i = 0; i < 6; i++) {
        if ((planes[i].normal.x * x + planes[i].normal.y * y + planes[i].normal.z * z) < (-radius - planes[i].d)) {
            return false;
        }
    }

    return true;
}

Frustum::~Frustum() {
    //void
}
