#pragma once

#include <string>
#include <memory>

namespace worldGame3D {
class ConnectedDeviceInformation;
class ClientAccountHandlerBase;

class IHandler_ClientAccountHandler {
public:
    virtual ConnectedDeviceInformation* getDeviceInformation() = 0;

    virtual void onDeviceConnect_ok(ClientAccountHandlerBase* target, int deviceId, std::shared_ptr<std::string>& deviceKey) = 0;
    virtual void onSessionConnect_ok(ClientAccountHandlerBase* target, int sessionId, std::shared_ptr<std::string>& sessionKey, int socialUserId, std::string& nickname) = 0;

    virtual void onAccountHandling_netError(ClientAccountHandlerBase* target) = 0;
    virtual void onAccountHandling_logicError(ClientAccountHandlerBase* target) = 0;

    virtual void onAccountInteraction_failed(ClientAccountHandlerBase* target) = 0;

    virtual ~IHandler_ClientAccountHandler() = default;
};
};
