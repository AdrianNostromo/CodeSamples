#pragma once

#include <base/gh.h>

namespace base {
class IHapticFeedbackManager {
    pub virtual void doHapticFeedback(int hapticFeedbackType) = 0;

    pub virtual ~IHapticFeedbackManager() = default;
};
};
