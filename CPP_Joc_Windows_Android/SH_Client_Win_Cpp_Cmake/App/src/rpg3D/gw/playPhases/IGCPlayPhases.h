#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>

namespace base {
    class GWEvent;
};
namespace rpg3D {
    namespace playPhases {
        class IPlayPhaseInteractor;
        class IPhaseEntity;
    };
};

namespace rpg3D {
namespace playPhases {
class IGCPlayPhases : virtual public IGamePart {priv typedef IGamePart super;
    pub static int TYPE;
    DEC_componentBasics();

    pub static base::GWEvent* GWEvent_turnChanged;

    pub virtual int getTurnIndex() = 0;

    pub virtual IPlayPhaseInteractor* registerTurnBasedEntity(IPhaseEntity* turnBasedEntity) = 0;

    pub virtual void setIsPlayPhasesEnabled(bool isPlayPhasesEnabled) = 0;

    pub virtual void onUserInput_endCurrentPlayPhase() = 0;

    pub ~IGCPlayPhases() override = default;
};
};
};
