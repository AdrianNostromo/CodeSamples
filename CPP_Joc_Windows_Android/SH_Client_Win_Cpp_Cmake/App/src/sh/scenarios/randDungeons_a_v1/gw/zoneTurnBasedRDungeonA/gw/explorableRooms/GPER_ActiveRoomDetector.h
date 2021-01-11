#pragma once

#include <base/gh.h>
#include "GPER_ActveRooms.h"

class IWorldEntity;
namespace base {
    class GWEvent;
    class EntityEvent;
};

namespace randDungeons_a_v1 {
namespace TurnBasedRDungeonA {
class GPER_ActiveRoomDetector : public GPER_ActveRooms {priv typedef GPER_ActveRooms super;pub dCtor(GPER_ActiveRoomDetector);
    priv IWorldEntity* playerEntity = nullptr;
    prot bool isDirty_playerPos = false;

    // This is initialised only by the local code that detects player pos changes.
    // This is not changed by preActivateRoomIfInnactive. The first room will be re-activated by the local code.
    priv int currentPlayerRoomIndex = -1;

    priv ArrayList<std::shared_ptr<IEventListener>> playerEntityAutoListenersList{};

    pub explicit GPER_ActiveRoomDetector(base::IGameWorld* gw);
    prot void createBMain() override;

    prot inline int getCurrentPlayerRoomIndex() {
        return currentPlayerRoomIndex;
    }

    priv void onPlayerEntityEvent(IEventListener& eventListener, base::EntityEvent& event);

    priv void onEvent(IEventListener& eventListener, base::GWEvent& event);
    priv void onGWEvent_gameLoop_contentCreation(IEventListener& eventListener, base::GWEvent& event);

    pub ~GPER_ActiveRoomDetector() override;
};
};
};
