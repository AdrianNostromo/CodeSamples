#include "Server.h"

using namespace worldGame3D;

Server::Server(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, base::IHttpRequestsManager* httpRequests, worldGame3D::IAppGame* app, base::Net::INativeSocketsHandler* nativeSocketsHandler)
    : super(logger, errorHandler, profileData, httpRequests, app, nativeSocketsHandler)
{
    //void
}

Server::~Server() {
    //void
}
