#pragma once

#include <base/gh.h>
#include "MDINTool.h"

namespace rpg3D {
class MDINative : public MDINTool {priv typedef MDINTool super; pub dCtor(MDINative);
    pub explicit MDINative(base::IGameWorld* gw);

    pub ~MDINative() override;
};
};
