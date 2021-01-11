#pragma once

#include <base/log/ILogHandler.h>
#include <base/net/util/IpPort.h>
#include <base/net/socket/util/Socket.h>
#include <base/httprequests/IHttpRequestsManager.h>
#include "ClientAccountHandlerActiveNetCon.h"
#include "IHandler_ClientAccountHandler.h"

namespace worldGame3D {
class ClientAccountHandler : public ClientAccountHandlerActiveNetCon {
private: typedef ClientAccountHandlerActiveNetCon super;
private:
    static const bool IS_DEBUG = false;

public:
    ClientAccountHandler(ClientAccountHandler const&) = delete;
    ClientAccountHandler(ClientAccountHandler &&) = default;
    ClientAccountHandler& operator=(ClientAccountHandler const&) = delete;
    ClientAccountHandler& operator=(ClientAccountHandler &&) = default;

    explicit ClientAccountHandler(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app);

    ~ClientAccountHandler() override;
protected:
    void create() override;

    void onAccountsInteractorIpPortReceived(std::shared_ptr<base::Net::IpPort> ipPort) override;
    void onSocketCreated(std::shared_ptr<base::Net::Socket> socket) override;

};
};
