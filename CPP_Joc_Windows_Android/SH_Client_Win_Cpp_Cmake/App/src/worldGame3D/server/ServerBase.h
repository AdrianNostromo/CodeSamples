#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include <base/component/IComponent_AppComponent.h>
#include <base/error/IErrorHandler.h>
#include "IServerPartHandler.h"
#include "IServer.h"
#include <base/log/ILogHandler.h>
#include <base/profiledata/IPlayerProfileDataLocal.h>
#include <worldGame3D/server/clientaccounthandler/IHandler_ClientAccountHandler.h>

namespace worldGame3D {
	class IAppGame;
};

namespace worldGame3D {
class ServerBase : public ComponentsHandler, virtual public IServerPartHandler, virtual public IServer, public virtual IComponent_AppComponent, virtual public IHandler_ClientAccountHandler {priv typedef ComponentsHandler super;pub dCtor(ServerBase);
    prot ILogHandler* logger;
    prot IErrorHandler* errorHandler;
    prot base::IPlayerProfileDataLocal* profileData;
    prot worldGame3D::IAppGame* app;

    pub explicit ServerBase(ILogHandler* logger, IErrorHandler* errorHandler, base::IPlayerProfileDataLocal* profileData, worldGame3D::IAppGame* app);

    pub ILogHandler* getLogger() override;

    pub ~ServerBase() override;
};
};
