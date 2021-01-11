#include <base/app/config/AppConfig.h>
#include "ServerAccount.h"
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>
#include <worldGame3D/app/IAppGame.h>

using namespace worldGame3D;

ServerAccount::ServerAccount(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, base::IHttpRequestsManager* httpRequests, worldGame3D::IAppGame* app, base::Net::INativeSocketsHandler* nativeSocketsHandler)
    : super(logger, errorHandler, profileData, app),
      httpRequests(httpRequests), nativeSocketsHandler(nativeSocketsHandler)
{
    //void
}

void ServerAccount::create() {
    super::create();

    app->getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_inputDispatch->type,
        std::bind(&ServerAccount::onLoopEvent_appLoop_general, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );
}

bool ServerAccount::getIsAccountConnectPrefferedState() {
    return isAccountConnectPrefferedState;
}

void ServerAccount::setIsAccountConnectPrefferedState(bool isAccountConnectPrefferedState) {
    this->isAccountConnectPrefferedState = isAccountConnectPrefferedState;
}

bool ServerAccount::isSessionConnectedOrConnecting() {
    if(isSessionConnected() || clientAccountHandler != nullptr) {
        return true;
    }

    return false;
}

void ServerAccount::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
    if(isAccountConnectPrefferedState) {
        if(!isSessionConnectedOrConnecting()) {
            reconnectDelayS -= event.deltaS;

            if(reconnectDelayS <= 0.0f) {
                reconnectDelayS = 0.0f;

                if(AppConfig::IS_DEBUG && IS_DEBUG) {
                    logger->postLog("Client account connect. Start.");
                }

                if(clientAccountHandler != nullptr) {
                    // Connection attempt in progress.

                    return ;
                }

                addComponent(clientAccountHandler = new ClientAccountHandler(
                    logger,
                    this,
                    nativeSocketsHandler,
                    app
                ), true/*doCreate*/);
            }
        }
    }

    if(markedForRemoval_clientAccountHandlersList.size() > 0) {
        disposeMarkedForRemoval();
    }
}

void ServerAccount::onDeviceConnect_ok(ClientAccountHandlerBase* target, int deviceId, std::shared_ptr<std::string>& deviceKey) {
    deviceConnectOccured(deviceId, deviceKey);
}

void ServerAccount::onSessionConnect_ok(ClientAccountHandlerBase* target, int sessionId, std::shared_ptr<std::string>& sessionKey, int socialUserId, std::string& nickname) {
    sessionConnectOccured(sessionId, sessionKey, socialUserId, nickname);

    markForRemoval_ClientAccountHandler();
}

void ServerAccount::onAccountHandling_netError(ClientAccountHandlerBase* target) {
    logger->postLog("Client account handling. Net error. Reconnection delay started.");

    markForRemoval_ClientAccountHandler();

    reconnectDelayS = 2.0f;
}

void ServerAccount::onAccountHandling_logicError(ClientAccountHandlerBase* target) {
    markForRemoval_ClientAccountHandler();

    errorHandler->handleAppCriticalError("Critical error", "ServerAccount.onAccountHandling_logicError.1.");
}

void ServerAccount::onAccountInteraction_failed(ClientAccountHandlerBase* target) {
    logger->postLog("Client account handling. Interaction failed. Clearing local connected account information. Reconnection delay started.");

    markForRemoval_ClientAccountHandler();

    onAccountInteractionFailed();

    reconnectDelayS = 2.0f;
}

void ServerAccount::userDisconnectOccuredDetected() {
    super::userDisconnectOccuredDetected();

    if(clientAccountHandler != nullptr) {
        markForRemoval_ClientAccountHandler();
    }

    logger->postLog("Account disconnected.");
}

void ServerAccount::markForRemoval_ClientAccountHandler() {
    if(clientAccountHandler != nullptr) {
        markedForRemoval_clientAccountHandlersList.appendReference(clientAccountHandler);

        clientAccountHandler = nullptr;
    }
}

void ServerAccount::disposeMarkedForRemoval() {
    while(markedForRemoval_clientAccountHandlersList.size() > 0) {
        ClientAccountHandler* comp = markedForRemoval_clientAccountHandlersList.removeAndGetDirect(0);

        removeComponent(comp, true/*doDispose*/);
    }
}

void ServerAccount::onAccountInteractionFailed() {
    super::onAccountInteractionFailed();

    //void
}

ServerAccount::~ServerAccount() {
    disposeMarkedForRemoval();
}
