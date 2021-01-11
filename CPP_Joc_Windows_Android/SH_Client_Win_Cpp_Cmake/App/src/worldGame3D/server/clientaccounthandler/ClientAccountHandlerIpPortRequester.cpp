#include <base/app/config/AppConfig.h>
#include "ClientAccountHandlerIpPortRequester.h"
#include <base/exceptions/LogicException.h>
#include <base/net/http/util/HttpMethods.h>
#include <base/util/json11/json11.h>
#include <worldGame3D/app/IAppGame.h>
#include <worldGame3D/server/IServer.h>
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace worldGame3D;

std::string ClientAccountHandlerIpPortRequester::ServerComponentID_accountesInteractor = "client_accounts_interactor";

ClientAccountHandlerIpPortRequester::ClientAccountHandlerIpPortRequester(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app)
    : super(logger, handler, nativeSocketsHandler, app)
{
    //void
}

void ClientAccountHandlerIpPortRequester::create() {
    super::create();

    app->getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_inputDispatch->type,
        std::bind(&ClientAccountHandlerIpPortRequester::onLoopEvent_appLoop_general, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );
}

void ClientAccountHandlerIpPortRequester::requestAccountsInteractorIpPort() {
    if(activeHttpRequest != nullptr) {
        throw LogicException(LOC);
    }

    activeHttpRequest = app->getServer()->getComponentIpPort()->poolServerComponentIpPort(
        ServerComponentID_accountesInteractor,
        nullptr
    );

    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postLog("Account_connect. Request account interactor ip port. Start.");
    }
}

void ClientAccountHandlerIpPortRequester::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
    if (activeHttpRequest != nullptr && activeHttpRequest->getIsFinished()) {
        int httpRequestErrorCode = activeHttpRequest->getFailCode();

        if (httpRequestErrorCode == IComponentIpPort::FailCodes::None) {
            if (AppConfig::IS_DEBUG && IS_DEBUG) {
                logger->postLog("Account_connect. Request account interactor ip port. Response received(validation is next).");
            }

            std::shared_ptr<base::Net::IpPort> ipPort = activeHttpRequest->getResponseIpPort();
            activeHttpRequest = nullptr;

            onAccountsInteractorIpPortReceived(ipPort);
        } else {
            activeHttpRequest = nullptr;

            if (httpRequestErrorCode == IComponentIpPort::FailCodes::Canceled) {
                if (AppConfig::IS_DEBUG && IS_DEBUG) {
                    logger->postLog("Account_connect. Request account interactor ip port. Canceled.");
                }

                //void
            } else if (httpRequestErrorCode == IComponentIpPort::FailCodes::LogicError) {
                if (AppConfig::IS_DEBUG && IS_DEBUG) {
                    logger->postLog("Account_connect. Request account interactor ip port. Logic error.");
                }

                handler->onAccountHandling_logicError(this);
            } else if (httpRequestErrorCode == IComponentIpPort::FailCodes::DataPoolFailed) {
                if (AppConfig::IS_DEBUG && IS_DEBUG) {
                    logger->postLog("Account_connect. Request account interactor ip port. Net error.");
                }

                handler->onAccountHandling_netError(this);
            } else if (httpRequestErrorCode < 0) {
                // Custom error code;
                if (AppConfig::IS_DEBUG && IS_DEBUG) {
                    logger->postLog("Account_connect. Request account interactor ip port. Net error (probably timed out).");
                }

                handler->onAccountHandling_netError(this);
            } else {
                throw LogicException(LOC);
            }
        }
    }
}

void ClientAccountHandlerIpPortRequester::disposeMain() {
    if(activeHttpRequest != nullptr) {
        activeHttpRequest->setListener(nullptr);
        activeHttpRequest->cancel();

        activeHttpRequest = nullptr;
    }

    super::disposeMain();
}

ClientAccountHandlerIpPortRequester::~ClientAccountHandlerIpPortRequester() {
    //void
}
