#pragma once

#include <base/gh.h>
#include <rpg3D/gw/ui/GamePartUserInterface.h>
#include "IGamePartUserInterface.h"

namespace startarea_a_v1 {
class GPUI_Base : public rpg3D::GamePartUserInterface, virtual public IGamePartUserInterface {priv typedef rpg3D::GamePartUserInterface super;pub dCtor(GPUI_Base);
    pub explicit GPUI_Base(base::IGameWorld* gw);

    pub ~GPUI_Base() override;
};
};
