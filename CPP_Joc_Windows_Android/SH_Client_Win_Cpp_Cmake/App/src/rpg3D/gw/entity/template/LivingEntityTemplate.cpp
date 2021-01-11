#include <rpg3D/gw/entity/module/LivingEntityModule.h>
#include "LivingEntityTemplate.h"

using namespace rpg3D;


LivingEntityTemplate::LivingEntityTemplate(float maxHP)
	: super("livingEntity", ILivingEntityModule::TYPE),
	  maxHP(maxHP)
{
	//void
}

IEntityModule* LivingEntityTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	LivingEntityTemplate* t = dynamic_cast<LivingEntityTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	LivingEntityModule* mod = new LivingEntityModule(entity, t);

	return mod;
}

LivingEntityTemplate::~LivingEntityTemplate() {
	//void
}
