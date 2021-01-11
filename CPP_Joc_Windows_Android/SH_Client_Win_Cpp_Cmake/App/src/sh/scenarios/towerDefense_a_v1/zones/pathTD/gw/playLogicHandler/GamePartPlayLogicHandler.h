#pragma once

#include <base/gh.h>
#include "GPPLH_PlayLoop.h"

namespace towerDefense_a_v1 {
namespace PathTD {
class GamePartPlayLogicHandler : public GPPLH_PlayLoop {priv typedef GPPLH_PlayLoop super;pub dCtor(GamePartPlayLogicHandler);
    pub explicit GamePartPlayLogicHandler(base::IGameWorld* gw);

    pub ~GamePartPlayLogicHandler() override;
};
};
};
