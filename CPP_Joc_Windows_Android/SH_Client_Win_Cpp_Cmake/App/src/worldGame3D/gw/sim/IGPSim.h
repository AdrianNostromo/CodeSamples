#pragma once

#include <worldGame3D/gw/event/GWEvent.h>
#include <base/math/Vector3.h>

namespace base {
class IGPSim {
public:
	static GWEvent* GWEvent_onCameraModeChanged;

public:
    virtual int getCameraMode() = 0;
    virtual void setCameraMode(int cameraModeBitGroup, bool animateTransition) = 0;

    virtual void configCamera_followerFirstPersonCamera(
	    Vector3& offsetDir, float offsetDist) = 0;
    virtual void configCamera_followerThirdPersonCamera(
	    Vector3& offsetDir,
	    float posOffset_dist_min, float posOffset_dist_max,
	    float minOffsetAtWindowDiagonalInches, float maxOffsetAtWindowDiagonalInches,
	    Vector3& focusPointOffset) = 0;

    virtual ~IGPSim() = default;
};
};
