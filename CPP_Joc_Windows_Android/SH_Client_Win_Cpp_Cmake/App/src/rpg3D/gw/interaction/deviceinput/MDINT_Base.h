#pragma once

#include <base/gh.h>
#include "MDIN_Sensors.h"

namespace rpg3D {
class MDINT_Base : public MDIN_Sensors {priv typedef MDIN_Sensors super;pub dCtor(MDINT_Base);
    pub explicit MDINT_Base(base::IGameWorld* gw);

    pub ~MDINT_Base() override;
};
};
