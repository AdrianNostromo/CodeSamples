#pragma once

#include <base/gh.h>
#include "GPER_ActiveRoomDetector.h"

namespace randDungeons_a_v1 {
namespace RDungeonA {
class GamePartExplorableRooms : public GPER_ActiveRoomDetector {priv typedef GPER_ActiveRoomDetector super;pub dCtor(GamePartExplorableRooms);
    pub explicit GamePartExplorableRooms(base::IGameWorld* gw);

    pub ~GamePartExplorableRooms() override;
};
};
};
