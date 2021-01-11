#pragma once

#include <base/gh.h>
#include "GPCC_Base.h"

namespace base {
    class GWEvent;
};

namespace rpg3D {
class GPCC_PlayerEntityStyle : public GPCC_Base {priv typedef GPCC_Base super;pub dCtor(GPCC_PlayerEntityStyle);
    pub explicit GPCC_PlayerEntityStyle(base::IGameWorld* gw);

    priv void onGWEvent_PlayerUnitMaterialsAndVoxelsGrid_loaded(IEventListener& eventListener, base::GWEvent& event);

    pub ~GPCC_PlayerEntityStyle() override;
};
};
