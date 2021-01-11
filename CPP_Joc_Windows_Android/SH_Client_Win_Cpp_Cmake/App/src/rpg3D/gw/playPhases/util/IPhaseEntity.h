#pragma once

#include <base/gh.h>

namespace rpg3D {
namespace playPhases {
class IPhaseEntity {
    pub virtual int getTeam() = 0;

    pub virtual void onCurrentActionChanged() = 0;

    pub virtual ~IPhaseEntity() = default;
};
};
};
