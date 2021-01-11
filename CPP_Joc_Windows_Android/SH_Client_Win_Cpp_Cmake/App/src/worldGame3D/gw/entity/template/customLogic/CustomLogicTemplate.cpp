#include "CustomLogicTemplate.h"
#include <worldGame3D/gw/entity/module/customLogic/CustomLogicModule.h>

using namespace base;

CustomLogicTemplate::CustomLogicTemplate(
	std::string moduleName, int moduleId,
	std::function<CustomLogicModule* (IWorldEntity* entity, CustomLogicTemplate* t)> moduleFactory)
    : super(moduleName, moduleId),
	moduleFactory(moduleFactory)
{
    //void
}

IEntityModule* CustomLogicTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
    CustomLogicTemplate* t = dynamic_cast<CustomLogicTemplate*>(_t);
    if(!t) {
        throw LogicException(LOC);
    }

	CustomLogicModule* mod = moduleFactory(entity, t);

    return mod;
}

CustomLogicTemplate::~CustomLogicTemplate() {
    //void
}
