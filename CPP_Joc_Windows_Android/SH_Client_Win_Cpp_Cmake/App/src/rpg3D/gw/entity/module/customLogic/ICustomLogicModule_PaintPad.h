#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>

namespace rpg3D {
class ICustomLogicModule_PaintPad : public virtual IEntityModule {priv typedef IComponent super;
    pub static int TYPE;
    DEC_componentBasics();

    pub ~ICustomLogicModule_PaintPad() override = default;
};
};
