#pragma once

#include <base/gh.h>
#include "MDINative.h"

namespace rpg3D {
class MDI_TouchArea2D : public MDINative {priv typedef MDINative super; pub dCtor(MDI_TouchArea2D);
    pub explicit MDI_TouchArea2D(base::IGameWorld* gw);

    pub ~MDI_TouchArea2D() override;
};
};
