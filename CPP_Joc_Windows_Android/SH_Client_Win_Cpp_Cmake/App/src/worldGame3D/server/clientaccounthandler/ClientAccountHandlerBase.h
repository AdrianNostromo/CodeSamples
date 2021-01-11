#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include <base/component/IComponent_AppComponent.h>
#include <base/log/ILogHandler.h>
#include <base/net/socket/INativeSocketsHandler.h>
#include "IHandler_ClientAccountHandler.h"

namespace worldGame3D {
	class IAppGame;
};

namespace worldGame3D {
class ClientAccountHandlerBase : public ComponentsHandler, public virtual IComponent_AppComponent {priv typedef ComponentsHandler super;pub dCtor(ClientAccountHandlerBase);
    priv static const bool IS_DEBUG = false;

    prot worldGame3D::IAppGame* app;

    prot ILogHandler* logger;

    prot IHandler_ClientAccountHandler* handler;
    prot base::Net::INativeSocketsHandler* nativeSocketsHandler;

    pub explicit ClientAccountHandlerBase(ILogHandler* logger, IHandler_ClientAccountHandler* handler, base::Net::INativeSocketsHandler* nativeSocketsHandler, worldGame3D::IAppGame* app);

    prot void disposeMain() override;

    pub ~ClientAccountHandlerBase() override;
};
};
