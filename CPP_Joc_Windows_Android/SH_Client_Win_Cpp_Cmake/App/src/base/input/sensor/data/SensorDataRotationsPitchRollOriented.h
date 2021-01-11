#pragma once

#include <base/gh.h>
#include "SensorData.h"

namespace base {
class SensorDataRotationsPitchRollOriented : public SensorData {priv typedef SensorData super;pub dCtor(SensorDataRotationsPitchRollOriented);
    pub class Type {
        pub static const int Data;
    };

    pub float pitchDeg;
    pub float pitchDeg_delta;

    pub float rollDeg;
    pub float rollDeg_delta;

    pub explicit SensorDataRotationsPitchRollOriented(float pitchDeg, float pitchDeg_delta, float rollDeg, float rollDeg_delta);

    pub ~SensorDataRotationsPitchRollOriented() override;
};
};
