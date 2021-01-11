#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGamePartContentZones.h"

namespace worldGame3D {
class GPCZ_Base : public GamePart, virtual public IGamePartContentZones {priv typedef GamePart super;pub dCtor(GPCZ_Base);
    pub explicit GPCZ_Base(base::IGameWorld* gw);

    pub ~GPCZ_Base() override;
};
};
