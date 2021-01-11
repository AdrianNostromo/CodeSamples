#include <rpg3D/gw/entity/module/controller/ControllerModule.h>
#include "ControllerTemplate.h"

using namespace rpg3D;

ControllerTemplate::ControllerTemplate()
    : super("controller", IControllerModule::TYPE)
{
    //void
}

IEntityModule* ControllerTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	ControllerTemplate* t = dynamic_cast<ControllerTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}
	
	ControllerModule* mod = new ControllerModule(entity, t);

	return mod;
}

ControllerTemplate::~ControllerTemplate() {
    //void
}
