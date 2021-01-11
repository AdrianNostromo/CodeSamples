#pragma once

#include <base/gh.h>
#include "SensorData.h"

namespace base {
class SensorDataKeyboard : public SensorData {priv typedef SensorData super;pub dCtor(SensorDataKeyboard);
    pub class Type {
        pub static const int Key;
    };

    pub int keyCode;
    pub int actionType;
    pub int modifierBits;

    pub explicit SensorDataKeyboard(int keyCode, int actionType, int modifierBits);

    pub ~SensorDataKeyboard() override;
};
};
