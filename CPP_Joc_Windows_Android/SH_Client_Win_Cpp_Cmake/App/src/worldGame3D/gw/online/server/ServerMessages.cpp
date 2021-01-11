#include "ServerMessages.h"

using namespace base;

ServerMessages::ServerMessages(std::shared_ptr<base::ServerConnectConfig> serverConnectConfig)
    : super(serverConnectConfig)
{
    //void
}

void ServerMessages::init_cb_processMsg(std::function<bool(
    base::Net::IGeneralTcpCon& con, int msgLen, int msgType, StreamBuffer& msgNetBuffer)> cb_processMsg)
{
    this->cb_processMsg = cb_processMsg;
}

bool ServerMessages::processMsg(base::Net::IGeneralTcpCon& con, int msgLen, int msgType, StreamBuffer& msgNetBuffer) {
    return cb_processMsg(con, msgLen, msgType, msgNetBuffer);
}

ServerMessages::~ServerMessages() {
    //void
}
