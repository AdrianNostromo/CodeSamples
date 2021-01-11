#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include <base/component/IComponent_ServerComponent.h>
#include "INetConsManager.h"
#include <worldGame3D/server/IServerPartHandler.h>

namespace worldGame3D {
class NetConsManagerBase : public base::ManagedObject, virtual public INetConsManager, public virtual IComponent_ServerComponent {priv typedef base::ManagedObject super;pub dCtor(NetConsManagerBase);
    prot IServerPartHandler* server;

    pub explicit NetConsManagerBase(IServerPartHandler* server);

    pub virtual ~NetConsManagerBase();
};
};
