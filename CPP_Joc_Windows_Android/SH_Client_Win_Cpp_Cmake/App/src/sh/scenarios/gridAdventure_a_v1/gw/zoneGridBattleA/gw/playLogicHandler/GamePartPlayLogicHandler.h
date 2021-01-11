#pragma once

#include <base/gh.h>
#include "GPPLH_Platforms.h"
#include <graphics/visual3d/drawable/IContainer3D.h>

namespace gridAdventure_a_v1 {
namespace GridBattleA {
class GamePartPlayLogicHandler : public GPPLH_Platforms {priv typedef GPPLH_Platforms super;pub dCtor(GamePartPlayLogicHandler);
    pub explicit GamePartPlayLogicHandler(base::IGameWorld* gw);

    pub ~GamePartPlayLogicHandler() override;
};
};
};
