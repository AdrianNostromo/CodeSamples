#include "EntityRemoveReason.h"

EntityRemoveReason EntityRemoveReason::SERVER_SYNC{"SERVER_SYNC", true, true};
EntityRemoveReason EntityRemoveReason::CODE_DISPOSED{"CODE_DISPOSED", false, false};

EntityRemoveReason::EntityRemoveReason(std::string _id, bool _doDeathSequence, bool _causesGameEvent)
        :id(_id), doDeathSequence(_doDeathSequence), causesGameEvent(_causesGameEvent)
{
    //void
}
