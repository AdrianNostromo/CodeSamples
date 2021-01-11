#include "WorldItemsCollectorTemplate.h"
#include <rpg3D/gw/entity/module/worldItemsCollector/WorldItemsCollectorModule.h>

using namespace rpg3D;

WorldItemsCollectorTemplate::WorldItemsCollectorTemplate()
    : super("worldItemsCollector", IWorldItemsCollectorModule::TYPE)
{
    //void
}

IEntityModule* WorldItemsCollectorTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    WorldItemsCollectorTemplate* t = dynamic_cast<WorldItemsCollectorTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    WorldItemsCollectorModule* mod = new WorldItemsCollectorModule(entity, t);

    return mod;
}

WorldItemsCollectorTemplate::~WorldItemsCollectorTemplate() {
    //void
}
