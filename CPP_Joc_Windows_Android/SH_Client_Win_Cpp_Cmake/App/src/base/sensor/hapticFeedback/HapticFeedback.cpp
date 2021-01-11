#include "HapticFeedback.h"
#include <base/nativeInput/IHapticFeedbackManager.h>

using namespace base;

HapticFeedback::HapticFeedback(base::IHapticFeedbackManager* manager, int hapticFeedbackType)
    : manager(manager), hapticFeedbackType(hapticFeedbackType)
{
    //void
}

void HapticFeedback::play() {
    manager->doHapticFeedback(hapticFeedbackType);
}

HapticFeedback::~HapticFeedback() {
    //void
}
