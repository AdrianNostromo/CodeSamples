#pragma once

#include "ServerAccount.h"

namespace worldGame3D {
class Server : public ServerAccount {
private: typedef ServerAccount super;
public:
    Server(Server const&) = delete;
    Server(Server &&) = default;
    Server& operator=(Server const&) = delete;
    Server& operator=(Server &&) = default;

    explicit Server(
		ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, 
		base::IHttpRequestsManager* httpRequests, worldGame3D::IAppGame* app,
		base::Net::INativeSocketsHandler* nativeSocketsHandler);

    virtual ~Server();
};
};
