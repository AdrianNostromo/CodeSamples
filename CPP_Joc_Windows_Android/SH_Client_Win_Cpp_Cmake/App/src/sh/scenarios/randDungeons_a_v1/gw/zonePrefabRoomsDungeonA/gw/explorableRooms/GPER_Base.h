#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGamePartExplorableRooms.h"

namespace randDungeons_a_v1 {
namespace PrefabRoomsDungeonA {
class GPER_Base : public GamePart, virtual public IGamePartExplorableRooms {priv typedef GamePart super;pub dCtor(GPER_Base);
    pub explicit GPER_Base(base::IGameWorld* gw);

    pub ~GPER_Base() override;
};
};
};
