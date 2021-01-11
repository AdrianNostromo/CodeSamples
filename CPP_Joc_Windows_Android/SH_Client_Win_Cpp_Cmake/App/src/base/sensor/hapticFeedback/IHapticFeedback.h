#pragma once

#include <base/gh.h>

namespace base {
class IHapticFeedback {
    pub virtual void play() = 0;

    pub virtual ~IHapticFeedback() = default;
};
};
