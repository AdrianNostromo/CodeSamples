#pragma once

#include <string>

class EntityRemoveReason {
public:
    static EntityRemoveReason SERVER_SYNC;
    static EntityRemoveReason CODE_DISPOSED;

    std::string id;

    bool doDeathSequence;
    bool causesGameEvent;

public:
    EntityRemoveReason(std::string _id, bool _doDeathSequence, bool _causesGameEvent);
};
