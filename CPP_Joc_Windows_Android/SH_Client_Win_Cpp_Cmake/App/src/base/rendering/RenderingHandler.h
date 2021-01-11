#pragma once

#include <base/gh.h>
#include "RH_List.h"

class RenderingHandler : public RH_List {priv typedef RH_List super; pub dCtor(RenderingHandler);
    pub RenderingHandler();

    pub ~RenderingHandler();
};
