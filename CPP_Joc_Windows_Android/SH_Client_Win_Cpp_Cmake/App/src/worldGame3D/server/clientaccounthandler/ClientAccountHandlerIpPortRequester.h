#pragma once

#include <base/gh.h>
#include <base/httprequests/IHttpRequestsManager.h>
#include <base/net/util/IpPort.h>
#include "ClientAccountHandlerBase.h"
#include <worldGame3D/server/componentipport/IComponentIpPort.h>
#include <base/list/ArrayList.h>

class IEventListener;
namespace base {
    class LoopEvent;
};

namespace worldGame3D {
class ClientAccountHandlerIpPortRequester : public ClientAccountHandlerBase {priv typedef ClientAccountHandlerBase super;pub dCtor(ClientAccountHandlerIpPortRequester);
    // EntryPointRequest
    pub static std::string ServerComponentID_accountesInteractor;

    priv static const bool IS_DEBUG = false;

    priv std::shared_ptr<IComponentIpPort::IRequest> activeHttpRequest = nullptr;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit ClientAccountHandlerIpPortRequester(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app);
    prot void create() override;

    prot virtual void onLoopEvent_appLoop_general(IEventListener& eventListener, base::LoopEvent& event);

    prot void requestAccountsInteractorIpPort();

    prot virtual void onAccountsInteractorIpPortReceived(std::shared_ptr<base::Net::IpPort> ipPort) = 0;

    prot void disposeMain() override;
    pub ~ClientAccountHandlerIpPortRequester() override;
};
};
