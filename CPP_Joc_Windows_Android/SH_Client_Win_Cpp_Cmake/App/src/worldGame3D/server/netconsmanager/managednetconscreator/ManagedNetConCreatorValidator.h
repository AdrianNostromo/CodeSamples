#pragma once

#include "ManagedNetConCreatorSocket.h"

namespace worldGame3D {
class ManagedNetConCreatorValidator : public ManagedNetConCreatorSocket {
private: typedef ManagedNetConCreatorSocket super;
public:
    ManagedNetConCreatorValidator(ManagedNetConCreatorValidator const&) = delete;
    ManagedNetConCreatorValidator(ManagedNetConCreatorValidator &&) = default;
    ManagedNetConCreatorValidator& operator=(ManagedNetConCreatorValidator const&) = delete;
    ManagedNetConCreatorValidator& operator=(ManagedNetConCreatorValidator &&) = default;

    explicit ManagedNetConCreatorValidator(IServerPartHandler* server, std::string& serverComponentId, IListener* listener);

    virtual ~ManagedNetConCreatorValidator();
};
};
