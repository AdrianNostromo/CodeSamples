#pragma once

#include "IHapticFeedback.h"
#include <base/gh.h>

namespace base {
    class IHapticFeedbackManager;
};

namespace base {
class HapticFeedback : virtual public IHapticFeedback {pub dCtor(HapticFeedback);
    prot base::IHapticFeedbackManager* manager;
    prot int hapticFeedbackType;

    pub explicit HapticFeedback(base::IHapticFeedbackManager* manager, int hapticFeedbackType);

    pub void play() override;

    pub ~HapticFeedback() override;
};
};
