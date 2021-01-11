#pragma once

#include "ServerDevice.h"

namespace worldGame3D {
class ServerSession : public ServerDevice {
private: typedef ServerDevice super;
private:
    ClientSessionInformation* sessionInformation = nullptr;

public:
    ServerSession(ServerSession const&) = delete;
    ServerSession(ServerSession &&) = default;
    ServerSession& operator=(ServerSession const&) = delete;
    ServerSession& operator=(ServerSession &&) = default;

    explicit ServerSession(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app);

    bool isSessionConnected() override;
    ClientSessionInformation* getSessionInformation() override;

    void sessionConnectOccured(int sessionId, std::shared_ptr<std::string> sessionKey, int socialUserId, std::string& nickname);

    void userDisconnectOccuredDetected() override;

    ~ServerSession() override;
};
};
