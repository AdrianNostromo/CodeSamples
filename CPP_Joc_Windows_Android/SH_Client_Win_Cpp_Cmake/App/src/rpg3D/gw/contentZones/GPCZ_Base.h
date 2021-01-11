#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/contentZones/GamePartContentZones.h>
#include "IGamePartContentZones.h"

namespace rpg3D {
class GPCZ_Base : public worldGame3D::GamePartContentZones, virtual public IGamePartContentZones {priv typedef worldGame3D::GamePartContentZones super;pub dCtor(GPCZ_Base);
    pub explicit GPCZ_Base(base::IGameWorld* gw);

    pub ~GPCZ_Base() override;
};
};
