#pragma once

#include <base/gh.h>

class VisualCreateConfig {pub dCtor(VisualCreateConfig);
    prot static int NewType();

    pub int type;

    pub explicit VisualCreateConfig(int type);

    pub virtual ~VisualCreateConfig();

};
