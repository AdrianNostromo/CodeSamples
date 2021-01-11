#pragma once

#include "IServer.h"
#include <memory>
#include <worldGame3D/gw/online/server/util/ServerConnectConfig.h>

namespace base {
class ServerBase : virtual public IServer {
private:
    std::shared_ptr<base::ServerConnectConfig> serverConnectConfig;

public:
    ServerBase(ServerBase const&) = delete;
    ServerBase(ServerBase &&) = default;
    ServerBase& operator=(ServerBase const&) = delete;
    ServerBase& operator=(ServerBase &&) = default;

    explicit ServerBase(std::shared_ptr<base::ServerConnectConfig> serverConnectConfig);
    void reservedCreate() final;

    void reservedDisposeMain() final;
    ~ServerBase() override;
protected:
    virtual void createMain();

    std::shared_ptr<base::ServerConnectConfig> getServerConnectConfig();

    virtual void disposeMain();
};
};
