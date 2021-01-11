#include <rpg3D/gw/entity/module/grid2DContent/Grid2DContentModule.h>
#include "Grid2DContentTemplate.h"

using namespace rpg3D;

Grid2DContentTemplate::Grid2DContentTemplate(
	bool isGridOrganisingGPMandatory)
	: super("grid2DContent", IGrid2DContentModule::TYPE),
	isGridOrganisingGPMandatory(isGridOrganisingGPMandatory)
{
    //void
}

IEntityModule* Grid2DContentTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	Grid2DContentTemplate* t = dynamic_cast<Grid2DContentTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	Grid2DContentModule* mod = new Grid2DContentModule(entity, t);

	return mod;
}

Grid2DContentTemplate::~Grid2DContentTemplate() {
    //void
}
