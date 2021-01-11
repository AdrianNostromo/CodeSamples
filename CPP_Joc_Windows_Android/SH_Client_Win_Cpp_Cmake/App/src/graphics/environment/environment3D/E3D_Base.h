#pragma once

#include <base/gh.h>
#include "../Environment.h"
#include "IEnvironment3D.h"

namespace graphics {
class E3D_Base : public Environment, virtual public IEnvironment3D {priv typedef Environment super;pub dCtor(E3D_Base);
    pub explicit E3D_Base();

    pub ~E3D_Base() override;
};
};
