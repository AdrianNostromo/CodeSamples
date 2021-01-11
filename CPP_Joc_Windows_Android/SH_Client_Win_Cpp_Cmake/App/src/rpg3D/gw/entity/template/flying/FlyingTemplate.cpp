#include "FlyingTemplate.h"
#include <rpg3D/gw/entity/module/flying/FlyingModule.h>

using namespace rpg3D;

FlyingTemplate::FlyingTemplate(
    float flyingSpeedS)
    : super("flying", IFlyingModule::TYPE),
	flyingSpeedS(flyingSpeedS)
{
    //void
}

IEntityModule* FlyingTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    FlyingTemplate* t = dynamic_cast<FlyingTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    FlyingModule* mod = new FlyingModule(entity, t);

    return mod;
}

FlyingTemplate::~FlyingTemplate() {
    //void
}
