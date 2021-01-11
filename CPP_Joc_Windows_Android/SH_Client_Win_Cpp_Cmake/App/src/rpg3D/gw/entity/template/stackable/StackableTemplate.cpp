#include "StackableTemplate.h"
#include <rpg3D/gw/entity/module/stackable/StackableModule.h>

using namespace rpg3D;

StackableTemplate::StackableTemplate(
	bool canHaul, bool canBeCarried,
	float haulOffsetZM,
	bool allowPlaceOnThis)
	: super("stackable", IStackableModule::TYPE),
	canHaul(canHaul), canBeCarried(canBeCarried),
	haulOffsetZM(haulOffsetZM),
	allowPlaceOnThis(allowPlaceOnThis)
{
	//void
}

IEntityModule* StackableTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	StackableTemplate* t = dynamic_cast<StackableTemplate*>(_t);
	if (!t) {
		throw LogicException(LOC);
	}

	StackableModule* mod = new StackableModule(entity, t);

	return mod;
}

StackableTemplate::~StackableTemplate() {
	//void
}
