#pragma once

#include <base/gh.h>
#include "LBGTBAT_TurnMonitor.h"

namespace rpg3D {
namespace playerUnit {
class LBGridTurnBasedActiveTools : public LBGTBAT_TurnMonitor {priv typedef LBGTBAT_TurnMonitor super; pub dCtor(LBGridTurnBasedActiveTools);
    pub explicit LBGridTurnBasedActiveTools();

    pub ~LBGridTurnBasedActiveTools() override;
};
};
};
