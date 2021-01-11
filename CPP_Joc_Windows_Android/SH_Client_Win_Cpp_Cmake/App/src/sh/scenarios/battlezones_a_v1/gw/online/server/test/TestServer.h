#pragma once

#include "TSBase.h"

namespace battlezones_a_v1 {
class TestServer : public TSBase {
private: typedef TSBase super;
private:
    bool isA = false;

public:
    TestServer(TestServer const&) = delete;
    TestServer(TestServer &&) = default;
    TestServer& operator=(TestServer const&) = delete;
    TestServer& operator=(TestServer &&) = default;

    explicit TestServer(std::shared_ptr<base::ServerConnectConfig> serverConnectConfig);

    virtual void onGameLoop_netInput(float deltaS, int deltaMS, float gameTimeS);

    ~TestServer() override;
};
};
