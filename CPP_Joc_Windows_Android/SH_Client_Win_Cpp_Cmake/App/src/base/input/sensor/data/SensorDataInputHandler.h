#pragma once

#include <base/gh.h>
#include "SensorData.h"

namespace base {
class SensorDataInputHandler : public SensorData {priv typedef SensorData super;pub dCtor(SensorDataInputHandler);
    pub class Type {
        pub static const int EventsCacheBufferDispatchFinished;
    };

    pub explicit SensorDataInputHandler();

    pub ~SensorDataInputHandler() override;
};
};
