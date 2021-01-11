#pragma once

#include "ServerBase.h"
#include <base/list/ArrayList.h>
#include <base/net/tcp/GeneralTcpConnection.h>
#include <functional>

namespace base {
class ServerMessages : public ServerBase {
private: typedef ServerBase super;
private:
    std::function<bool(
        base::Net::IGeneralTcpCon& con, int msgLen, int msgType, StreamBuffer& msgNetBuffer)> cb_processMsg;

public:
    ServerMessages(ServerMessages const&) = delete;
    ServerMessages(ServerMessages &&) = default;
    ServerMessages& operator=(ServerMessages const&) = delete;
    ServerMessages& operator=(ServerMessages &&) = default;

    explicit ServerMessages(std::shared_ptr<base::ServerConnectConfig> serverConnectConfig);

    void init_cb_processMsg(std::function<bool(
        base::Net::IGeneralTcpCon& con, int msgLen, int msgType, StreamBuffer& msgNetBuffer)> cb_processMsg);

    ~ServerMessages() override;
protected:
    bool processMsg(base::Net::IGeneralTcpCon& con, int msgLen, int msgType, StreamBuffer& msgNetBuffer);

};
};
