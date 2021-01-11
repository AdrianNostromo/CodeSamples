#include "ClientAccountHandlerNetConnector.h"
#include <base/exceptions/LogicException.h>
#include <iostream>
#include <base/app/config/AppConfig.h>
#include <base/app/IApp.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/appLoop/event/LoopEvent.h>

using namespace worldGame3D;

ClientAccountHandlerNetConnector::ClientAccountHandlerNetConnector(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app)
    : super(logger, handler, nativeSocketsHandler, app)
{
    //void
}

void ClientAccountHandlerNetConnector::createNewSocket(std::string& ip, int port) {
    if(socketCreator != nullptr) {
        throw LogicException(LOC);
    }

    if(AppConfig::IS_DEBUG && IS_DEBUG) {
        logger->postLog("Client account handling. Creating socket.");
    }
    socketConnectDurationS = 0.0f;
    socketCreator = nativeSocketsHandler->newNativeSocketCreator(ip, port);
    socketCreator->start();
}

void ClientAccountHandlerNetConnector::onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) {
    super::onLoopEvent_appLoop_general(eventListener, event);

    if(socketCreator != nullptr) {
        if(socketCreator->getIsFinished()) {
            int socketErrorCode = socketCreator->getErrorCode();

            if(socketErrorCode == base::Net::INativeSocketCreator::ErrorCode::None) {
                if(AppConfig::IS_DEBUG && IS_DEBUG) {
                    logger->postLog("Client account handling. Socket created.");
                }

                std::shared_ptr<base::Net::INativeSocket> nativeSocket = socketCreator->extractNativeSocket();

                socketCreator = nullptr;

                std::shared_ptr<base::Net::Socket> newSocket = std::make_shared<base::Net::Socket>(nativeSocket);
                newSocket->reservedCreate();

                onSocketCreated(newSocket);
            }else {
                socketCreator = nullptr;

                if (socketErrorCode == base::Net::INativeSocketCreator::ErrorCode::Canceled) {
                    //void
                } else if (socketErrorCode == base::Net::INativeSocketCreator::ErrorCode::LogicError) {
                    if(AppConfig::IS_DEBUG && IS_DEBUG) {
                        logger->postLog("Client account handling. Socket logic error.");
                    }

                    handler->onAccountHandling_logicError(this);
                } else if (socketErrorCode == base::Net::INativeSocketCreator::ErrorCode::NetError) {
                    if(AppConfig::IS_DEBUG && IS_DEBUG) {
                        logger->postLog("Client account handling. Socket network error.");
                    }

                    handler->onAccountHandling_netError(this);
                } else {
                    throw LogicException(LOC);
                }
            }
        }else {
            socketConnectDurationS += event.deltaS;
            if(socketConnectDurationS >= 5.0f) {
                if(AppConfig::IS_DEBUG && IS_DEBUG) {
                    logger->postLog("Client account handling. Socket logic error.");
                }

                socketCreator = nullptr;

                handler->onAccountHandling_logicError(this);
            }
        }
    }
}

void ClientAccountHandlerNetConnector::disposeMain() {
    if(socketCreator != nullptr) {
        socketCreator->cancel();

        socketCreator = nullptr;
    }

    super::disposeMain();
}

ClientAccountHandlerNetConnector::~ClientAccountHandlerNetConnector() {
    //void
}
