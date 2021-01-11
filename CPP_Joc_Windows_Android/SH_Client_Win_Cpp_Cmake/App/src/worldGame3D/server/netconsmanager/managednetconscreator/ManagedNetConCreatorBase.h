#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include "IManagedNetConsCreator.h"
#include <string>
#include <worldGame3D/server/IServerPartHandler.h>

namespace worldGame3D {
class ManagedNetConCreatorBase : public base::ManagedObject, virtual public IManagedNetConsCreator {priv typedef base::ManagedObject super;pub dCtor(ManagedNetConCreatorBase);
    prot IServerPartHandler* server;

    prot std::string serverComponentId;

    priv IListener* listener;

    //asdA3; priv Object userData;

    pub explicit ManagedNetConCreatorBase(IServerPartHandler* server, std::string& serverComponentId, IListener* listener);

    pub virtual ~ManagedNetConCreatorBase();
};
};
