#pragma once

#include <base/gh.h>
#include <base/MM.h>
#include "IGCPlayPhases.h"
#include <worldGame3D/gw/gamepart/GamePart.h>
#include <base/sp.h>
#include <base/list/Array1D.h>
#include <base/list/ArrayList.h>

namespace rpg3D {
    namespace playPhases {
        class TurnPhaseConfig;
    };
};

namespace rpg3D {
namespace playPhases {
class GCPP_Base : public GamePart, virtual public IGCPlayPhases {priv typedef GamePart super;pub dCtor(GCPP_Base);
    prot sp<Array1D<TurnPhaseConfig>> playPhaseConfigsList;
    
    pub explicit GCPP_Base(
        base::IGameWorld* gw,
        sp<Array1D<TurnPhaseConfig>> playPhaseConfigsList);

    pub ~GCPP_Base() override;
};
};
};
