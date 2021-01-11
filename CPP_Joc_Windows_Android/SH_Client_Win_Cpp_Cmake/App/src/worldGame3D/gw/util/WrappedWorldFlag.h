#pragma once

#include <base/gh.h>
#include "WorldFlag.h"

namespace worldGame3D {
class WrappedWorldFlag {pub dCtor(WrappedWorldFlag);
    pub WorldFlag* worldFlag;

    pub explicit WrappedWorldFlag(WorldFlag* worldFlag);

    pub ~WrappedWorldFlag();
};
};
