#pragma once

#include <base/gh.h>
#include <base/input/sensor/data/SensorData.h>

namespace rpg3D {
class SensorDataToolExtraActivation : public base::SensorData {priv typedef base::SensorData super;pub dCtor(SensorDataToolExtraActivation);
    pub class Type {
        pub static const int Data_ListControl_Down_MainBtn;
        pub static const int Data_SecondaryControl_Down_MainBtn;
    };

    pub int toolInstanceId;
    pub int sourceId;

    pub explicit SensorDataToolExtraActivation(int toolInstanceId, int sourceId);

    pub ~SensorDataToolExtraActivation() override;
};
};
