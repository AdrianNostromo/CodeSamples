#pragma once

#include <base/gh.h>

namespace rpg3D {
class ITurnBasedEntity {
    pub virtual int getTeam() = 0;

    pub virtual void onCurrentActionChanged() = 0;

    pub virtual ~ITurnBasedEntity() = default;
};
};
