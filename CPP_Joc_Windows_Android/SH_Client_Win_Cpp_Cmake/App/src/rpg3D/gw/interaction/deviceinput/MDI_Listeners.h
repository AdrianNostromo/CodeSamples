#pragma once

#include <base/gh.h>
#include "MDI_Listeners_General.h"

namespace rpg3D {
class MDI_Listeners : public MDI_Listeners_General {priv typedef MDI_Listeners_General super; pub dCtor(MDI_Listeners);
    pub explicit MDI_Listeners(base::IGameWorld* gw);

    pub ~MDI_Listeners() override;
};
};
