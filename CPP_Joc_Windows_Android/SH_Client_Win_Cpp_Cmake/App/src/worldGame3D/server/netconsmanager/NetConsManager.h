#pragma once

#include "NetConsManagerActiveConnections.h"

namespace worldGame3D {
class NetConsManager : public NetConsManagerActiveConnections {
private: typedef NetConsManagerActiveConnections super;
public:
    NetConsManager(NetConsManager const&) = delete;
    NetConsManager(NetConsManager &&) = default;
    NetConsManager& operator=(NetConsManager const&) = delete;
    NetConsManager& operator=(NetConsManager &&) = default;

    explicit NetConsManager(IServerPartHandler* server);

    virtual ~NetConsManager();
};
};
