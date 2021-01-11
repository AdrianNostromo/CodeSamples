#include <base/app/config/AppConfig.h>
#include "ServerSession.h"

using namespace worldGame3D;

ServerSession::ServerSession(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app)
    : super(logger, errorHandler, profileData, app)
{
    //void
}

bool ServerSession::isSessionConnected() {
    if(sessionInformation != nullptr) {
        return true;
    }

    return false;
}

ClientSessionInformation* ServerSession::getSessionInformation() {
    return sessionInformation;
}

void ServerSession::sessionConnectOccured(int sessionId, std::shared_ptr<std::string> sessionKey, int socialUserId, std::string& nickname) {
    sessionInformation = new ClientSessionInformation(
        sessionId,
        sessionKey,

        socialUserId,
        nickname
    );

    if(AppConfig::IS_DEBUG && IS_DEBUG_ACCOUNT) {
        logger->postDebug(std::string("Account. Session connected (online), ") + "id: " + std::to_string(sessionInformation->sessionId)
            + ", nickname: " + sessionInformation->nickname
        );
    }
}

void ServerSession::userDisconnectOccuredDetected() {
    super::userDisconnectOccuredDetected();

    //Clear session information.
    sessionInformation = nullptr;

    if(AppConfig::IS_DEBUG && IS_DEBUG_ACCOUNT) {
       logger->postDebug("Account. Session: disconnected.");
    }
}

ServerSession::~ServerSession() {
    //void
}
