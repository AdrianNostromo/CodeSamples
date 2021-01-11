#pragma once

#include "ManagedNetConCreatorBase.h"

namespace worldGame3D {
class ManagedNetConCreatorIpPort : public ManagedNetConCreatorBase {
private: typedef ManagedNetConCreatorBase super;
public:
    ManagedNetConCreatorIpPort(ManagedNetConCreatorIpPort const&) = delete;
    ManagedNetConCreatorIpPort(ManagedNetConCreatorIpPort &&) = default;
    ManagedNetConCreatorIpPort& operator=(ManagedNetConCreatorIpPort const&) = delete;
    ManagedNetConCreatorIpPort& operator=(ManagedNetConCreatorIpPort &&) = default;

    explicit ManagedNetConCreatorIpPort(IServerPartHandler* server, std::string& serverComponentId, IListener* listener);

    virtual ~ManagedNetConCreatorIpPort();
};
};
