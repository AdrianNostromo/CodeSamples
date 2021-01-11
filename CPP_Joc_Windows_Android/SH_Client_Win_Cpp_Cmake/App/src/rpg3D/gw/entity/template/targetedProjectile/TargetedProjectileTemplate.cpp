#include <rpg3D/gw/entity/module/targetedProjectile/TargetedProjectileModule.h>
#include "TargetedProjectileTemplate.h"

using namespace rpg3D;

TargetedProjectileTemplate::TargetedProjectileTemplate(
	std::string* impactSoundConfig_id, float impactSoundConfig_volume)
	: super("targetedProjectile", ITargetedProjectileModule::TYPE),
	impactSoundConfig_id(impactSoundConfig_id), impactSoundConfig_volume(impactSoundConfig_volume)
{
    //void
}

IEntityModule* TargetedProjectileTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	TargetedProjectileTemplate* t = dynamic_cast<TargetedProjectileTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	TargetedProjectileModule* mod = new TargetedProjectileModule(entity, t);

	return mod;
}

TargetedProjectileTemplate::~TargetedProjectileTemplate() {
    //void
}
