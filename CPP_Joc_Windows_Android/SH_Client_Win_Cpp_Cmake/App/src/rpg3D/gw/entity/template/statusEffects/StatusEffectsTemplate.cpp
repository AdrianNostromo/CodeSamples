#include <rpg3D/gw/entity/module/statusEffects/StatusEffectsModule.h>
#include "StatusEffectsTemplate.h"

using namespace rpg3D;

StatusEffectsTemplate::StatusEffectsTemplate()
	: super("statusEffects", IStatusEffectsModule::TYPE)
{
	//void
}

IEntityModule* StatusEffectsTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	StatusEffectsTemplate* t = dynamic_cast<StatusEffectsTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	StatusEffectsModule* mod = new StatusEffectsModule(entity, t);

	return mod;
}

StatusEffectsTemplate::~StatusEffectsTemplate() {
	//void
}
