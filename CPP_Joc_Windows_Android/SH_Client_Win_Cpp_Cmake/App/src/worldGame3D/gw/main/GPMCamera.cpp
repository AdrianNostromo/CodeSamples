#include "GPMCamera.h"
#include <base/math/collision/Intersector.h>
#include <base/screen/ScreenUtil.h>
#include <graphics/environment/environment3D/Environment3D.h>

using namespace base;

GPMCamera::GPMCamera(base::IGameWorld* gw)
    : super(gw)
{
    //void
}

bool GPMCamera::camera2DCoordsProjectionTo3DGroundPos(Vector2& screenPos, float groundPlanePosZ, Vector3* oIntersectionPos, Vector3* oRayDirection) {
    Ray& ray = getEnvironment3D()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
	if (isnan(ray.origin.x)) {
		// This occured when the window was minimised.
		Ray& ray1 = getEnvironment3D()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
		Ray& ray2 = getEnvironment3D()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
		Ray& ray3 = getEnvironment3D()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);
		throw LogicException(LOC);
	}

	if (oRayDirection != nullptr) {
		oRayDirection->set(ray.direction);
	}

	Plane groundPlane{
		Vector3::Z, 
		groundPlanePosZ
	};

    Vector3 intersectionPoint{};
    if (Intersector::intersectRayPlane(ray, groundPlane, &intersectionPoint)) {
		if (isnan(intersectionPoint.x)) {
			// This occured when the window was minimised.
			throw LogicException(LOC);
		}

		if (oIntersectionPos != nullptr) {
			oIntersectionPos->set(intersectionPoint.x, intersectionPoint.y, intersectionPoint.z);
		}

        return true;
    }

    return false;
}

void GPMCamera::camera2DCoordsProjectionGetRayDir(Vector2& screenPos, Vector3* oRayDirection) {
	Ray& ray = getEnvironment3D()->peekCamera()->getPickRay(screenPos.x, screenPos.y, true);

	if (oRayDirection != nullptr) {
		oRayDirection->set(ray.direction);
	}
}

GPMCamera::~GPMCamera() {
    //void
}
