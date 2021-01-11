#pragma once

#include "ManagedNetConCreatorStatus.h"

namespace worldGame3D {
class ManagedNetConCreator : public ManagedNetConCreatorStatus {
private: typedef ManagedNetConCreatorStatus super;
public:
    ManagedNetConCreator(ManagedNetConCreator const&) = delete;
    ManagedNetConCreator(ManagedNetConCreator &&) = default;
    ManagedNetConCreator& operator=(ManagedNetConCreator const&) = delete;
    ManagedNetConCreator& operator=(ManagedNetConCreator &&) = default;

    explicit ManagedNetConCreator(IServerPartHandler* server, std::string& serverComponentId, IListener* listener);

    virtual ~ManagedNetConCreator();
};
};
