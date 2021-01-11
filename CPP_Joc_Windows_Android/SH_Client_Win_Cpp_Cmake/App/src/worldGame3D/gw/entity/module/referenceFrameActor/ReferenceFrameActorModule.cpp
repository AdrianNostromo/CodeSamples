#include "ReferenceFrameActorModule.h"
#include <worldGame3D/gw/entity/module/referenceFrame/ReferenceFrameModule.h>
#include <base/exceptions/LogicException.h>

ReferenceFrameActorModule::ReferenceFrameActorModule(
    IWorldEntity* entity,
    ReferenceFrameActorTemplate* t
)
    : super(t->moduleName, entity),
      t(t)
{
    //void
}

IReferenceFrameModule* ReferenceFrameActorModule::getReferenceFrame() {
    return referenceFrame;
}

void ReferenceFrameActorModule::setReferenceFrame(IReferenceFrameModule* referenceFrame) {
    if((this->referenceFrame == nullptr && referenceFrame == nullptr) ||
        (this->referenceFrame != nullptr && referenceFrame != nullptr))
    {
        throw LogicException(LOC);
    }

    this->referenceFrame = referenceFrame;
}

ReferenceFrameActorModule::~ReferenceFrameActorModule() {
    //void
}
