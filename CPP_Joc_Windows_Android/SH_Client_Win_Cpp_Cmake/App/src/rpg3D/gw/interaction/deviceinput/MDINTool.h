#pragma once

#include <base/gh.h>
#include "MDINT_UIToolsExtraActivations.h"

namespace rpg3D {
class MDINTool : public MDINT_UIToolsExtraActivations {priv typedef MDINT_UIToolsExtraActivations super;pub dCtor(MDINTool);
    pub explicit MDINTool(base::IGameWorld* gw);

    pub ~MDINTool() override;
};
};
