#include <worldGame3D/gw/entity/module/referenceFrame/ReferenceFrameModule.h>
#include "ReferenceFrameTemplate.h"

ReferenceFrameTemplate::ReferenceFrameTemplate(
    std::string actorsLayerId
)
    : super("referenceFrame", IReferenceFrameModule::TYPE),
      actorsLayerId(actorsLayerId)
{
    //void
}

IEntityModule* ReferenceFrameTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    ReferenceFrameTemplate* t = dynamic_cast<ReferenceFrameTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    ReferenceFrameModule* mod = new ReferenceFrameModule(entity, t);

    return mod;
}

ReferenceFrameTemplate::~ReferenceFrameTemplate() {
    //void
}
