#pragma once

#include "GPMainBase.h"

namespace base {
class GPMCamera : public GPMainBase {
private: typedef GPMainBase super;
public:
    GPMCamera(GPMCamera const&) = delete;
    GPMCamera(GPMCamera &&) = default;
    GPMCamera& operator=(GPMCamera const&) = delete;
    GPMCamera& operator=(GPMCamera &&) = default;

    explicit GPMCamera(base::IGameWorld* gw);

    bool camera2DCoordsProjectionTo3DGroundPos(Vector2& screenPos, float groundPlanePosZ, Vector3* oIntersectionPos, Vector3* oRayDirection) override;
    void camera2DCoordsProjectionGetRayDir(Vector2& screenPos, Vector3* oRayDirection) override;

    ~GPMCamera() override;
};
};
