#include "StateChangeDurations.h"

StateChangeDurations::StateChangeDurations(std::string startComposedStateID, std::string endComposedStateID, float durationS)
    : durationS(durationS)
{
    this->startComposedStateID = UiId::newIdFromComposedId(startComposedStateID);
    this->endComposedStateID = UiId::newIdFromComposedId(endComposedStateID);
}

StateChangeDurations::~StateChangeDurations() {
    //void
}
