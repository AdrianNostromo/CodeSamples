#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IReferenceFrameActorModule.h"
#include <worldGame3D/gw/entity/template/referenceFrame/ReferenceFrameActorTemplate.h>

class IWorldEntity;

class ReferenceFrameActorModule : public EntityModule, public virtual IReferenceFrameActorModule {priv typedef EntityModule super;pub dCtor(ReferenceFrameActorModule);
    pub ReferenceFrameActorTemplate* t;

    pub IReferenceFrameModule* referenceFrame = nullptr;

    pub explicit ReferenceFrameActorModule(
        IWorldEntity* entity,
        ReferenceFrameActorTemplate* t
    );

    pub IReferenceFrameModule* getReferenceFrame() final;
    pub void setReferenceFrame(IReferenceFrameModule* referenceFrame) final;

    pub ~ReferenceFrameActorModule();
};
