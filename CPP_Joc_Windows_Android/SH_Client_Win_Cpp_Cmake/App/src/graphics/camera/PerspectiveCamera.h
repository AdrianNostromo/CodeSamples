#pragma once

#include <base/gh.h>
#include "Camera.h"

class PerspectiveCamera : public Camera {priv typedef Camera super;priv dCtor(PerspectiveCamera);
    /** the field of view of the height, in degrees **/
    priv float fieldOfViewY = 67.0f;

    priv Vector3 tmp{};

    pub explicit PerspectiveCamera(float fieldOfViewY);

    pub void setFieldOfViewY(float fieldOfViewY);

    prot void updateMain() override;

    pub ~PerspectiveCamera() override;
};
