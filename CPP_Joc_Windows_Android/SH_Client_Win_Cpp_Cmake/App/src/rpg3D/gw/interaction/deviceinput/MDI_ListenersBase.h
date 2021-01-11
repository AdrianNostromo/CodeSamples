#pragma once

#include <base/gh.h>
#include "MDI_Base.h"

namespace rpg3D {
class MDI_ListenersBase : public MDI_Base {priv typedef MDI_Base super; pub dCtor(MDI_ListenersBase);
    pub explicit MDI_ListenersBase(base::IGameWorld* gw);

    pub ~MDI_ListenersBase() override;
};
};
