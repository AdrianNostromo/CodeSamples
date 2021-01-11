#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IPlatformInteractorModule.h"
#include <rpg3D/gw/entity/template/platformInteractor/PlatformInteractorTemplate.h>
#include <base/list/ArrayList.h>

class IWorldEntity;

namespace rpg3D {
    class IPlatformModule;
};

namespace rpg3D {
class PlatformInteractorModule : public EntityModule, public virtual IPlatformInteractorModule {priv typedef EntityModule super;pub dCtor(PlatformInteractorModule);
    pub PlatformInteractorTemplate* t;

    pub explicit PlatformInteractorModule(
        IWorldEntity* entity,
        PlatformInteractorTemplate* t);

    pub ~PlatformInteractorModule() override;
};
};
