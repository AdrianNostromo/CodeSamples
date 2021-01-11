#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <base/input/util/Keyboard.h>
#include "MDI_TouchArea2D.h"

namespace rpg3D {
class ManagedDeviceInput : public MDI_TouchArea2D {priv typedef MDI_TouchArea2D super; pub dCtor(ManagedDeviceInput);
    pub explicit ManagedDeviceInput(base::IGameWorld* gw);

    pub ~ManagedDeviceInput() override;
};
};
