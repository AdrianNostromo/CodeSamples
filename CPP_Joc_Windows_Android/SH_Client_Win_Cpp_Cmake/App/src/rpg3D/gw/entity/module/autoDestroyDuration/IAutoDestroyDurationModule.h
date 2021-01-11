#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>

namespace rpg3D {
class IAutoDestroyDurationModule : public virtual IEntityModule {priv typedef IComponent super;
    pub static int TYPE;
    DEC_componentBasics();

    pub ~IAutoDestroyDurationModule() override = default;
};
};
