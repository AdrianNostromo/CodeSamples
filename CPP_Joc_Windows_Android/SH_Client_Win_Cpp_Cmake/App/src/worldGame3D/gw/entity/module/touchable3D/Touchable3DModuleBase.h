#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/template/touchable3D/Touchable3DTemplate.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "ITouchable3DModule.h"

class IWorldEntity;

class Touchable3DModuleBase : public EntityModule, public virtual ITouchable3DModule {priv typedef EntityModule super;pub dCtor(Touchable3DModuleBase);
    pub Touchable3DTemplate* t;

    pub explicit Touchable3DModuleBase(
        IWorldEntity* entity,
        Touchable3DTemplate* t
    );
    
    pub ~Touchable3DModuleBase();
};
