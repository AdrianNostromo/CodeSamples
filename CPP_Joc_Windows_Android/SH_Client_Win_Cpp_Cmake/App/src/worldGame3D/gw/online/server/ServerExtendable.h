#pragma once

#include "ServerMessages.h"

namespace base {
class ServerExtendable : public ServerMessages {
private: typedef ServerMessages super;
public:
    ServerExtendable(ServerExtendable const&) = delete;
    ServerExtendable(ServerExtendable &&) = default;
    ServerExtendable& operator=(ServerExtendable const&) = delete;
    ServerExtendable& operator=(ServerExtendable &&) = default;

    explicit ServerExtendable(std::shared_ptr<base::ServerConnectConfig> serverConnectConfig);

    ~ServerExtendable() override;
};
};
