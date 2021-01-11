#include "ClientAccountHandlerBase.h"
#include <base/component/IComponent_ClientAccountHandlerComponent.h>

using namespace worldGame3D;

ClientAccountHandlerBase::ClientAccountHandlerBase(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app)
    : super(IComponent_ClientAccountHandlerComponent::COMPONENT_CATEGORY),
    app(app), logger(logger), handler(handler), nativeSocketsHandler(nativeSocketsHandler)
{
    //void
}

void ClientAccountHandlerBase::disposeMain() {
    if(handler != nullptr) {
        handler = nullptr;
    }

    super::disposeMain();
}

ClientAccountHandlerBase::~ClientAccountHandlerBase() {
    //void
}
