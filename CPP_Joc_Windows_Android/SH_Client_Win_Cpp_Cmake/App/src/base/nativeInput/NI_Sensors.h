#pragma once

#include <base/gh.h>
#include "NI_Base.h"

namespace base {
class NI_Sensors : public NI_Base {priv typedef NI_Base super;pub dCtor(NI_Sensors);
    pub explicit NI_Sensors();

    pub bool getIsSensorActive(base::Touch::SensorType* sensorId) override;
    pub bool getIsSensorSupported(base::Touch::SensorType* sensorId) override;
    pub void setIsSensorActiveIncremental(base::Touch::SensorType* sensorId, bool isActive) override;

    pub ~NI_Sensors() override;
};
};
