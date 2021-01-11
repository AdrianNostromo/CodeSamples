#pragma once

#include <base/gh.h>
#include "SensorData.h"

namespace base {
class SensorDataAccelerometer : public SensorData {priv typedef SensorData super;pub dCtor(SensorDataAccelerometer);
    pub class Type {
        pub static const int Data;
    };

    pub float forceX;
    pub float forceY;
    pub float forceZ;

    pub explicit SensorDataAccelerometer(float forceX, float forceY, float forceZ);

    pub ~SensorDataAccelerometer() override;
};
};
