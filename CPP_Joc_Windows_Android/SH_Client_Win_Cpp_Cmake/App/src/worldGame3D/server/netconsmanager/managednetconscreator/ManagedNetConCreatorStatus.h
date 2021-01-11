#pragma once

#include "ManagedNetConCreatorValidator.h"

namespace worldGame3D {
class ManagedNetConCreatorStatus : public ManagedNetConCreatorValidator {
private: typedef ManagedNetConCreatorValidator super;
public:
    ManagedNetConCreatorStatus(ManagedNetConCreatorStatus const&) = delete;
    ManagedNetConCreatorStatus(ManagedNetConCreatorStatus &&) = default;
    ManagedNetConCreatorStatus& operator=(ManagedNetConCreatorStatus const&) = delete;
    ManagedNetConCreatorStatus& operator=(ManagedNetConCreatorStatus &&) = default;

    explicit ManagedNetConCreatorStatus(IServerPartHandler* server, std::string& serverComponentId, IListener* listener);

    virtual ~ManagedNetConCreatorStatus();
};
};
