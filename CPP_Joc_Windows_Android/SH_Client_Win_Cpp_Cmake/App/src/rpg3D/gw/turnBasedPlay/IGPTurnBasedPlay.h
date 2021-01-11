#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/IGamePart.h>

namespace base {
    class GWEvent;
};
namespace rpg3D {
    class ITurnBasedInteractor;
    class ITurnBasedEntity;
};

namespace rpg3D {
class IGPTurnBasedPlay : virtual public IGamePart {priv typedef IGamePart super;
    pub static int TYPE;
        DEC_componentBasics();

    pub static base::GWEvent* GWEvent_turnChanged;

    pub virtual int getTurnIndex() = 0;

    pub virtual ITurnBasedInteractor* registerTurnBasedEntity(ITurnBasedEntity* turnBasedEntity) = 0;

    pub virtual void setIsTurnBasedPlayEnabled(bool isTurnBasedPlayEnabled) = 0;

    pub ~IGPTurnBasedPlay() override = default;
};
};
