#pragma once

#include <base/gh.h>

namespace rpg3D {
    class IWorldInteractionFilter;
};

namespace rpg3D {
class IUserInputDispatcher {
    pub virtual void hookCustomFilter(rpg3D::IWorldInteractionFilter* filter) = 0;
    pub virtual void releaseCustomFilter(rpg3D::IWorldInteractionFilter* filter) = 0;

    pub virtual ~IUserInputDispatcher() = default;
};
};
