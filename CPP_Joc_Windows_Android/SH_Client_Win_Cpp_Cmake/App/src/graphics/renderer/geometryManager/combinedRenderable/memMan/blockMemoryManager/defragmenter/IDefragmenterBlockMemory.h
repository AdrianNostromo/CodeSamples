#pragma once

#include <base/gh.h>

namespace graphics {
class IDefragmenterBlockMemory {
    pub virtual void tickDefrag() = 0;

    pub virtual ~IDefragmenterBlockMemory() = default;
};
};
