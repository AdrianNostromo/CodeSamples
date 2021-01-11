#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/entity/module/EntityModule.h>
#include "IReferenceFrameModule.h"
#include <worldGame3D/gw/entity/module/referenceFrameActor/ReferenceFrameActorModule.h>
#include <worldGame3D/gw/entity/template/referenceFrame/ReferenceFrameTemplate.h>
#include <graphics/visual3d/drawable/IContainer3D.h>
#include <base/list/ArrayList.h>

class IWorldEntity;
class IReferenceFrameActorModule;

class ReferenceFrameModule : public EntityModule, public virtual IReferenceFrameModule {priv typedef EntityModule super;
    pub ReferenceFrameTemplate* t;

    pub IContainer3D* actorsLayer = nullptr;

    pub ArrayList<IReferenceFrameActorModule*> actorsList{};

    pub explicit ReferenceFrameModule(
        IWorldEntity* entity,
        ReferenceFrameTemplate* t
    );
    prot void createBMain() override;

    pub void addActor(
        IReferenceFrameActorModule* referenceFrameActor,
        float localPosX, float localPosY, float localPosZ
    );
    pub void removeActor(IReferenceFrameActorModule* referenceFrameActor);

    pub ~ReferenceFrameModule() override;
};
