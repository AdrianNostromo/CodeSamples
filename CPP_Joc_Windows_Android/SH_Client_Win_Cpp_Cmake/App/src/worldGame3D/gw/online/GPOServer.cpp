#include "GPOServer.h"

using namespace base;

GPOServer::GPOServer(IGameWorld* gw)
    : super(gw)
{
    //void
}

void GPOServer::createMain() {
    super::createMain();

    if(gw->getScenarioConfig()->serverConnectConfig) {
        server = newServer();
        server->init_cb_processMsg(std::bind(&GPOServer::processMsg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
        server->reservedCreate();
    }
}

ServerExtendable* GPOServer::newServer() {
    // Must be extended. This server is optional even for online apps.
    throw LogicException(LOC);
}

bool GPOServer::processMsg(base::Net::IGeneralTcpCon& con, int msgLen, int msgType, StreamBuffer& msgNetBuffer) {
    return false;
}

void GPOServer::disposeMain() {
    if(server != nullptr) {
        server->reservedDisposeMain();

        delete server;
        server = nullptr;
    }

    super::disposeMain();
}

GPOServer::~GPOServer() {
    //void
}
