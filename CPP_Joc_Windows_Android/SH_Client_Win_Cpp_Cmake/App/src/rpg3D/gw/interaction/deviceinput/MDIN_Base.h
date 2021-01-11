#pragma once

#include <base/gh.h>
#include "MDI_Listeners.h"

namespace rpg3D {
class MDIN_Base : public MDI_Listeners {priv typedef MDI_Listeners super; pub dCtor(MDIN_Base);
    pub explicit MDIN_Base(base::IGameWorld* gw);

    pub ~MDIN_Base() override;
};
};
