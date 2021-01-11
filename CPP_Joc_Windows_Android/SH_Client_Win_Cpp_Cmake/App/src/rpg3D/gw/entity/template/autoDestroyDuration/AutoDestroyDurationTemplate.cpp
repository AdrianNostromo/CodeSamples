#include "AutoDestroyDurationTemplate.h"
#include <rpg3D/gw/entity/module/autoDestroyDuration/AutoDestroyDurationModule.h>

using namespace rpg3D;

AutoDestroyDurationTemplate::AutoDestroyDurationTemplate(
    float lifeDistM, float lifeDurationS)
    : super("autoDestroyDuration", IAutoDestroyDurationModule::TYPE),
	lifeDistM(lifeDistM), lifeDurationS(lifeDurationS)
{
    //void
}

IEntityModule* AutoDestroyDurationTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    AutoDestroyDurationTemplate* t = dynamic_cast<AutoDestroyDurationTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    AutoDestroyDurationModule* mod = new AutoDestroyDurationModule(entity, t);

    return mod;
}

AutoDestroyDurationTemplate::~AutoDestroyDurationTemplate() {
    //void
}
