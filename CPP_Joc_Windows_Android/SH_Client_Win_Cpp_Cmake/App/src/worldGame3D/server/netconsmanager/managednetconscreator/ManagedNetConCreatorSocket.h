#pragma once

#include "ManagedNetConCreatorIpPort.h"

namespace worldGame3D {
class ManagedNetConCreatorSocket : public ManagedNetConCreatorIpPort{
private: typedef ManagedNetConCreatorIpPort super;
public:
    ManagedNetConCreatorSocket(ManagedNetConCreatorSocket const&) = delete;
    ManagedNetConCreatorSocket(ManagedNetConCreatorSocket &&) = default;
    ManagedNetConCreatorSocket& operator=(ManagedNetConCreatorSocket const&) = delete;
    ManagedNetConCreatorSocket& operator=(ManagedNetConCreatorSocket &&) = default;

    explicit ManagedNetConCreatorSocket(IServerPartHandler* server, std::string& serverComponentId, IListener* listener);

    virtual ~ManagedNetConCreatorSocket();
};
};
