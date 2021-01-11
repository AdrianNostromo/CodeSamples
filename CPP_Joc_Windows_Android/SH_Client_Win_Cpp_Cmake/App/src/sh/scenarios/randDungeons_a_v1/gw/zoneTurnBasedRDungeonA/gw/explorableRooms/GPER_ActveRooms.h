#pragma once

#include <base/gh.h>
#include "GPER_RoomConfigs.h"
#include <base/list/ListDL.h>

namespace rpg3D {
    class EffectPayload;
};

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class GPER_ActveRooms : public GPER_RoomConfigs {priv typedef GPER_RoomConfigs super;pub dCtor(GPER_ActveRooms);
    pub explicit GPER_ActveRooms(base::IGameWorld* gw);

    pub void preActivateRoomIfInnactive(int roomIndex) final;

    prot void activateRoom(Room& room, bool alsoCreateDoorNeighbourRooms);
    pub ~GPER_ActveRooms() override;
};
};
};
