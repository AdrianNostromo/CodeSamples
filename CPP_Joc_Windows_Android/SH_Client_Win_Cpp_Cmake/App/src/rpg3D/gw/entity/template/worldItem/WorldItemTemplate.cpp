#include "WorldItemTemplate.h"
#include <rpg3D/gw/entity/module/worldItem/WorldItemModule.h>

using namespace rpg3D;

WorldItemTemplate::WorldItemTemplate(
    std::string* impactSoundConfig_id, float impactSoundConfig_volume)
    : super("worldItem", IWorldItemModule::TYPE),
    impactSoundConfig_id(impactSoundConfig_id), impactSoundConfig_volume(impactSoundConfig_volume)
{
    //void
}

IEntityModule* WorldItemTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    WorldItemTemplate* t = dynamic_cast<WorldItemTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

    WorldItemModule* mod = new WorldItemModule(entity, t);

    return mod;
}

WorldItemTemplate::~WorldItemTemplate() {
    //void
}
