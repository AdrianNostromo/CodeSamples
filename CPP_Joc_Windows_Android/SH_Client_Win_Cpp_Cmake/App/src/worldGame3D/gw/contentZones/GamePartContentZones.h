#pragma once

#include <base/gh.h>
#include "GPCZ_ActiveZone.h"
#include "IGamePartContentZones.h"

namespace worldGame3D {
class GamePartContentZones : public GPCZ_ActiveZone, virtual public IGamePartContentZones {priv typedef GPCZ_ActiveZone super;pub dCtor(GamePartContentZones);
    pub explicit GamePartContentZones(base::IGameWorld* gw);

    pub ~GamePartContentZones() override;
};
};
