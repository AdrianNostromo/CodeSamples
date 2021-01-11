#pragma once

#include <base/gh.h>
#include <base/input/sensor/data/SensorData.h>
#include <base/input/util/Touch.h>

namespace base {
class SensorEvent {pub dCtor(SensorEvent);
    pub base::Touch::SensorType* sensorType;
    pub int eventType;

    // Managed by this.
    pub SensorData* data;

    pub explicit SensorEvent(base::Touch::SensorType* sensorType, int eventType, SensorData* data);

    pub inline int getTypeA() {
        return sensorType->index;
    }
    pub inline int getTypeB() {
        return eventType;
    }

    pub virtual ~SensorEvent();
};
};
