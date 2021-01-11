#pragma once

#include <base/gh.h>
#include "NI_Sensors.h"

namespace base {
class NativeInput : public NI_Sensors {priv typedef NI_Sensors super;pub dCtor(NativeInput);
    pub explicit NativeInput();

    pub ~NativeInput() override;
};
};
