#pragma once

#include <base/gh.h>
#include <base/net/socket/util/Socket.h>
#include <base/net/socket/util/INativeSocketCreator.h>
#include "ClientAccountHandlerIpPortRequester.h"

class IEventListener;
namespace base {
    class LoopEvent;
};

namespace worldGame3D {
class ClientAccountHandlerNetConnector : public ClientAccountHandlerIpPortRequester {priv typedef ClientAccountHandlerIpPortRequester super;pub dCtor(ClientAccountHandlerNetConnector);
    priv static const bool IS_DEBUG = false;

    priv std::shared_ptr<base::Net::INativeSocketCreator> socketCreator = nullptr;
    priv float socketConnectDurationS = 0.0f;

    pub explicit ClientAccountHandlerNetConnector(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app);

    pub void createNewSocket(std::string& ip, int port);

    prot void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event) override;

    prot virtual void onSocketCreated(std::shared_ptr<base::Net::Socket> socket) = 0;

    prot void disposeMain() override;
    pub ~ClientAccountHandlerNetConnector() override;
};
};
