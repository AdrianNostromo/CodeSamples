#include <rpg3D/gw/entity/module/platform/PlatformModule.h>
#include "PlatformTemplate.h"

using namespace rpg3D;

PlatformTemplate::PlatformTemplate()
    : super("platform", IPlatformModule::TYPE)
{
    //void
}

IEntityModule* PlatformTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    PlatformTemplate* t = dynamic_cast<PlatformTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    PlatformModule* mod = new PlatformModule(entity, t);

    return mod;
}

PlatformTemplate::~PlatformTemplate() {
    //void
}
