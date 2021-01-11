#include <worldGame3D/gw/entity/module/actions/ActionsModule.h>
#include "ActionsTemplate.h"

using namespace worldGame3D;

ActionsTemplate::ActionsTemplate()
	: super("actions", IActionsModule::TYPE)
{
	//void
}

IEntityModule* ActionsTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	ActionsTemplate* t = dynamic_cast<ActionsTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	ActionsModule* mod = new ActionsModule(entity, t);

	return mod;
}

ActionsTemplate::~ActionsTemplate() {
	//void
}
