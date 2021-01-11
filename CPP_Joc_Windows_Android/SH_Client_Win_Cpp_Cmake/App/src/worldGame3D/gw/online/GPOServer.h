#pragma once

#include <worldGame3D/gw/online/server/ServerExtendable.h>
#include <worldGame3D/gw/online/server/util/ServerConnectConfig.h>
#include "GPOBase.h"
#include <base/net/tcp/GeneralTcpConnection.h>

namespace base {
class GPOServer : public GPOBase {
private: typedef GPOBase super;
private:
    ServerExtendable* server = nullptr;

public:
    GPOServer(GPOServer const&) = delete;
    GPOServer(GPOServer &&) = default;
    GPOServer& operator=(GPOServer const&) = delete;
    GPOServer& operator=(GPOServer &&) = default;

    explicit GPOServer(IGameWorld* gw);

    ~GPOServer() override;
protected:
    void createMain() override;

    virtual ServerExtendable* newServer();

    virtual bool processMsg(base::Net::IGeneralTcpCon& con, int msgLen, int msgType, StreamBuffer& msgNetBuffer);

    void disposeMain() override;

};
};
