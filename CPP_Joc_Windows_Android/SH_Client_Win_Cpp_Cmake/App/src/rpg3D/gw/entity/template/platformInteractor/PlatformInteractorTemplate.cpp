#include <rpg3D/gw/entity/module/platformInteractor/PlatformInteractorModule.h>
#include "PlatformInteractorTemplate.h"

using namespace rpg3D;

PlatformInteractorTemplate::PlatformInteractorTemplate()
    : super("platformInteractor", IPlatformInteractorModule::TYPE)
{
    //void
}

IEntityModule* PlatformInteractorTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    PlatformInteractorTemplate* t = dynamic_cast<PlatformInteractorTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    PlatformInteractorModule* mod = new PlatformInteractorModule(entity, t);

    return mod;
}

PlatformInteractorTemplate::~PlatformInteractorTemplate() {
    //void
}
