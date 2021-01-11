#pragma once

#include "IServer.h"

class ILogHandler;

namespace worldGame3D {
class IComponentIpPort;

class IServerPartHandler : virtual public IServer {
public:
    virtual IComponentIpPort* getComponentIpPort() = 0;

    virtual ILogHandler* getLogger() = 0;

    virtual ~IServerPartHandler() = default;
};
};
