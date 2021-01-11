#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include <worldGame3D/server/IServerPartHandler.h>

class IApp;

namespace worldGame3D {
class ServerPart : public base::ManagedObject {priv typedef base::ManagedObject super;pub dCtor(ServerPart);
    prot IServerPartHandler* server;
    prot IApp* app;

    pub explicit ServerPart(IServerPartHandler* server, IApp* app);

    pub virtual ~ServerPart();
};
};
