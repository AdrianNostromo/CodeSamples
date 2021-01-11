#pragma once

#include <base/gh.h>
#include "../Environment.h"
#include "IEnvironment2D.h"

namespace graphics {
class Environment2D : public Environment, virtual public IEnvironment2D {priv typedef Environment super;pub dCtor(Environment2D);
    pub explicit Environment2D();

    prot void onColorCameraViewportSizeChanged() override;

    pub ~Environment2D() override;
};
};
