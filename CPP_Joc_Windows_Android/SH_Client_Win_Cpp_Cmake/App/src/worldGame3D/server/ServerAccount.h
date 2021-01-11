#pragma once

#include <base/gh.h>
#include <worldGame3D/server/clientaccounthandler/ClientAccountHandler.h>
#include <base/httprequests/IHttpRequestsManager.h>
#include "ServerSession.h"

class IEventListener;
namespace base {
    class LoopEvent;
};

namespace worldGame3D {
class ServerAccount : public ServerSession {priv typedef ServerSession super;pub dCtor(ServerAccount);
    prot base::IHttpRequestsManager* httpRequests;
    prot base::Net::INativeSocketsHandler* nativeSocketsHandler;

    priv static const bool IS_DEBUG = true;

    priv bool isAccountConnectPrefferedState = false;
    priv float reconnectDelayS = 0.0f;

    priv ClientAccountHandler* clientAccountHandler = nullptr;
    priv ArrayList<ClientAccountHandler*> markedForRemoval_clientAccountHandlersList{};

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit ServerAccount(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, base::IHttpRequestsManager* httpRequests, worldGame3D::IAppGame* app, base::Net::INativeSocketsHandler* nativeSocketsHandler);
    prot void create() override;

    pub bool getIsAccountConnectPrefferedState() override;
    pub void setIsAccountConnectPrefferedState(bool isAccountConnectPrefferedState) override;

    pub bool isSessionConnectedOrConnecting() override;

    prot virtual void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event);

    pub void onDeviceConnect_ok(ClientAccountHandlerBase* target, int deviceId, std::shared_ptr<std::string>& deviceKey) override;
    pub void onSessionConnect_ok(ClientAccountHandlerBase* target, int sessionId, std::shared_ptr<std::string>& sessionKey, int socialUserId, std::string& nickname) override;

    pub void onAccountHandling_netError(ClientAccountHandlerBase* target) override;
    pub void onAccountHandling_logicError(ClientAccountHandlerBase* target) override;

    pub void onAccountInteraction_failed(ClientAccountHandlerBase* target) override;

    pub void userDisconnectOccuredDetected() override;

    prot void onAccountInteractionFailed() override;

    priv void markForRemoval_ClientAccountHandler();
    priv void disposeMarkedForRemoval();

    pub ~ServerAccount() override;
};
};
