#include <worldGame3D/gw/entity/module/referenceFrameActor/ReferenceFrameActorModule.h>
#include "ReferenceFrameActorTemplate.h"

ReferenceFrameActorTemplate::ReferenceFrameActorTemplate()
    : super("referenceFrameActor", IReferenceFrameActorModule::TYPE)
{
    //void
}

IEntityModule* ReferenceFrameActorTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    ReferenceFrameActorTemplate* t = dynamic_cast<ReferenceFrameActorTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    ReferenceFrameActorModule* mod = new ReferenceFrameActorModule(entity, t);

    return mod;
}

ReferenceFrameActorTemplate::~ReferenceFrameActorTemplate() {
    //void
}
