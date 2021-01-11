#include <base/app/config/AppConfig.h>
#include <base/net/tcp/GeneralTcpConnection.h>
#include "ClientAccountHandler.h"

using namespace worldGame3D;

ClientAccountHandler::ClientAccountHandler(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app)
    : super(logger, handler, nativeSocketsHandler, app)
{
    //void
}

void ClientAccountHandler::create() {
    super::create();
    
    requestAccountsInteractorIpPort();
}

void ClientAccountHandler::onAccountsInteractorIpPortReceived(std::shared_ptr<base::Net::IpPort> ipPort) {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postLog("DevLog. Account. Ip port response received.");
    }

    createNewSocket(ipPort->ip, ipPort->port);
}

void ClientAccountHandler::onSocketCreated(std::shared_ptr<base::Net::Socket> socket) {
    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postLog("DevLog. Account. Socket created.");
    }

    std::shared_ptr<base::Net::GeneralTcpConnection> con = std::make_shared<base::Net::GeneralTcpConnection>(socket);
    newConCreated(con);
}

ClientAccountHandler::~ClientAccountHandler() {
    //void
}
