#include "ReferenceFrameModule.h"
#include <worldGame3D/gw/entity/module/referenceFrameActor/ReferenceFrameActorModule.h>
#include <worldGame3D/gw/entity/module/visual3D/IVisual3DModule.h>
#include <base/exceptions/LogicException.h>
#include <worldGame3D/gw/entity/IWorldEntity.h>
#include <worldGame3D/gw/entity/template/visual3D/Visual3DTemplate.h>

ReferenceFrameModule::ReferenceFrameModule(
    IWorldEntity* entity,
    ReferenceFrameTemplate* t
)
    : super(t->moduleName, entity),
      t(t)
{
    //void
}

void ReferenceFrameModule::addActor(
    IReferenceFrameActorModule* referenceFrameActor,
    float localPosX, float localPosY, float localPosZ) {
    if(referenceFrameActor->getReferenceFrame()) {
        throw LogicException(LOC);
    }

    // Add to a local list.
    actorsList.appendReference(referenceFrameActor);

    // Set this refFrame to the target.
    referenceFrameActor->setReferenceFrame(this);

    // Add the unit to this ref frame layer.
    worldGame3D::IVisual3DModule* refActor_visual3D = referenceFrameActor->getEntity()->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
    if(refActor_visual3D->getVisualsParent() != nullptr) {
        refActor_visual3D->setVisualsParent(nullptr);
    }
    referenceFrameActor->getEntity()->setPos(localPosX, localPosY, localPosZ);
    refActor_visual3D->setVisualsParent(actorsLayer);
}

void ReferenceFrameModule::removeActor(IReferenceFrameActorModule* referenceFrameActor) {
    if(referenceFrameActor->getReferenceFrame() != this) {
        throw LogicException(LOC);
    }

    worldGame3D::IVisual3DModule* refActor_visual3D = referenceFrameActor->getEntity()->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);
    refActor_visual3D->setVisualsParent(nullptr);

    if(!actorsList.remove(referenceFrameActor)) {
        throw LogicException(LOC);
    }

    referenceFrameActor->setReferenceFrame(nullptr);
}

void ReferenceFrameModule::createBMain() {
    super::createBMain();

    worldGame3D::IVisual3DModule* visual3D = entity->getComponentAs<worldGame3D::IVisual3DModule*>(true/*mustExist*/);

    actorsLayer = visual3D->getDrawableOptionalAs<IContainer3D*>(t->actorsLayerId);
}

ReferenceFrameModule::~ReferenceFrameModule() {
    //void
}
