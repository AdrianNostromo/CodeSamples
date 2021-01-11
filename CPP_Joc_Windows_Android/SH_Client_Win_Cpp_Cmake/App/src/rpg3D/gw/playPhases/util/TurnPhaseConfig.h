#pragma once

#include <base/gh.h>
#include <string>

namespace rpg3D {
namespace playPhases {
class TurnPhaseConfig {pub dCtor(TurnPhaseConfig);
    pub std::string name;

    pub bool isUserEndable;

    pub explicit TurnPhaseConfig(
        std::string name,
        bool isUserEndable);

    pub ~TurnPhaseConfig();
};
};
};
