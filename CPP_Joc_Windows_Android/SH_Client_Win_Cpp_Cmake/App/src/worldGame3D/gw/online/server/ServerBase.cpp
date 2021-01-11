#include "ServerBase.h"

using namespace base;

ServerBase::ServerBase(std::shared_ptr<base::ServerConnectConfig> serverConnectConfig)
    : serverConnectConfig(serverConnectConfig)
{
    //void
}

void ServerBase::reservedCreate() {
    createMain();
}

void ServerBase::createMain() {
    //void
}

std::shared_ptr<base::ServerConnectConfig> ServerBase::getServerConnectConfig() {
    return serverConnectConfig;
}

void ServerBase::reservedDisposeMain() {
    disposeMain();
}

void ServerBase::disposeMain() {
    //void
}

ServerBase::~ServerBase() {
    //void
}
