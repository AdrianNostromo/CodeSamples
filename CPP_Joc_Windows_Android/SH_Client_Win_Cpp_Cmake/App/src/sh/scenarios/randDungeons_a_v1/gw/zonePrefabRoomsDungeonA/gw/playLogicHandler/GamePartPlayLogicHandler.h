#pragma once

#include <base/gh.h>
#include "GPPLH_Platforms.h"

namespace randDungeons_a_v1 {
namespace PrefabRoomsDungeonA {
class GamePartPlayLogicHandler : public GPPLH_Platforms {priv typedef GPPLH_Platforms super;pub dCtor(GamePartPlayLogicHandler);
    pub explicit GamePartPlayLogicHandler(base::IGameWorld* gw);

    pub ~GamePartPlayLogicHandler() override;
};
};
};
