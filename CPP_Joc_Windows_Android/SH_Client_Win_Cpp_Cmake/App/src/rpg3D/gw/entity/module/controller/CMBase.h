#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include <rpg3D/gw/entity/template/controller/ControllerTemplate.h>
#include "IControllerModule.h"

namespace rpg3D {
class CMBase : public EntityModule, virtual public IControllerModule {priv typedef EntityModule super;pub dCtor(CMBase);
    prot ControllerTemplate* t;

    pub explicit CMBase(
	    IWorldEntity* entity,
	    ControllerTemplate* t);

    pub ~CMBase() override;
};
};
