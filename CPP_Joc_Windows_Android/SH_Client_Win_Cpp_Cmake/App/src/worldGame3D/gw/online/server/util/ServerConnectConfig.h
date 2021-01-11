#pragma once

namespace base {
class ServerConnectConfig {
public:
    ServerConnectConfig(ServerConnectConfig const&) = delete;
    ServerConnectConfig(ServerConnectConfig &&) = default;
    ServerConnectConfig& operator=(ServerConnectConfig const&) = delete;
    ServerConnectConfig& operator=(ServerConnectConfig &&) = default;

    explicit ServerConnectConfig();

    virtual ~ServerConnectConfig();
};
};
