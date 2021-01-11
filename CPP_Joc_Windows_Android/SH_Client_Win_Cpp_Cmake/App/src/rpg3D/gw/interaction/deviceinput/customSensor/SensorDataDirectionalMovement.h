#pragma once

#include <base/gh.h>
#include <base/input/sensor/data/SensorData.h>
#include <base/math/Vector2.h>

namespace rpg3D {
class SensorDataDirectionalMovement : public base::SensorData {priv typedef base::SensorData super;pub dCtor(SensorDataDirectionalMovement);
    pub class Type {
        pub static const int Data;
    };

    pub Vector2 moveDirInput;

    pub explicit SensorDataDirectionalMovement(Vector2& moveDirInput);

    pub ~SensorDataDirectionalMovement() override;
};
};
