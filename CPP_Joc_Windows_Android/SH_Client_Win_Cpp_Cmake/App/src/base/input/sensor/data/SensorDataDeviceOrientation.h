#pragma once

#include <base/gh.h>
#include "SensorData.h"
#include <base/input/util/Touch.h>

namespace base {
class SensorDataDeviceOrientation : public SensorData {priv typedef SensorData super;pub dCtor(SensorDataDeviceOrientation);
    pub class Type {
        pub static const int Data;
    };

    pub base::Touch::DeviceOrientation* deviceOrientation;

    pub explicit SensorDataDeviceOrientation(base::Touch::DeviceOrientation* deviceOrientation);

    pub ~SensorDataDeviceOrientation() override;
};
};
