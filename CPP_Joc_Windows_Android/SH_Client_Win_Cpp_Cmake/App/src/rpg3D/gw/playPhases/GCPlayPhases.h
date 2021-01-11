#pragma once

#include <base/gh.h>
#include "GCPP_Phases.h"

namespace rpg3D {
namespace playPhases {
class GCPlayPhases : public GCPP_Phases {priv typedef GCPP_Phases super;pub dCtor(GCPlayPhases);
    pub explicit GCPlayPhases(
        base::IGameWorld* gw,
        sp<Array1D<TurnPhaseConfig>> playPhaseConfigsList);

    pub ~GCPlayPhases() override;
};
};
};
