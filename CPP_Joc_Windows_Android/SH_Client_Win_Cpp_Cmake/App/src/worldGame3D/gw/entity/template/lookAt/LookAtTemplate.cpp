#include <worldGame3D/gw/entity/module/lookAt/LookAtModule.h>
#include "LookAtTemplate.h"

LookAtTemplate::LookAtTemplate()
    : super("lookAt", ILookAtModule::TYPE)
{
    //void
}

IEntityModule* LookAtTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    LookAtTemplate* t = dynamic_cast<LookAtTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    LookAtModule* mod = new LookAtModule(entity, t);

    return mod;
}

LookAtTemplate::~LookAtTemplate() {
    //void
}
