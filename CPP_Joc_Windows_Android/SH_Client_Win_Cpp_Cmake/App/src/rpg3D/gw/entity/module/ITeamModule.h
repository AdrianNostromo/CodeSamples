#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/IEntityModule.h>
#include <base/math/Vector3.h>

namespace rpg3D {
class ITeamModule : public virtual IEntityModule {priv typedef IComponent super;
    pub static int TYPE;
    DEC_componentBasics();

    pub virtual int getTeamIndex() = 0;
    pub virtual void setTeamIndex(int teamIndex) = 0;

    pub ~ITeamModule() override = default;
};
};
