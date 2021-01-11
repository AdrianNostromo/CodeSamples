#pragma once

#include "NetConsManagerCreator.h"

namespace worldGame3D {
class NetConsManagerActiveConnections : public NetConsManagerCreator {
private: typedef NetConsManagerCreator super;
public:
    NetConsManagerActiveConnections(NetConsManagerActiveConnections const&) = delete;
    NetConsManagerActiveConnections(NetConsManagerActiveConnections &&) = default;
    NetConsManagerActiveConnections& operator=(NetConsManagerActiveConnections const&) = delete;
    NetConsManagerActiveConnections& operator=(NetConsManagerActiveConnections &&) = default;

    explicit NetConsManagerActiveConnections(IServerPartHandler* server);

    virtual ~NetConsManagerActiveConnections();
};
};
