#include <rpg3D/gw/entity/module/toolsHandler/ToolsHandlerModule.h>
#include "ToolsHandlerTemplate.h"

using namespace rpg3D;

ToolsHandlerTemplate::ToolsHandlerTemplate(Array1D<ToolSlotConfig*>* toolSlotsList)
	: super("toolsHandler", IToolsHandlerModule::TYPE),
	  toolSlotsList(toolSlotsList)
{
	//void
}

IEntityModule* ToolsHandlerTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	ToolsHandlerTemplate* t = dynamic_cast<ToolsHandlerTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	ToolsHandlerModule* mod = new ToolsHandlerModule(entity, t);

	return mod;
}

ToolsHandlerTemplate::~ToolsHandlerTemplate() {
	//void
}
