#include <rpg3D/gw/entity/module/TeamModule.h>
#include "TeamTemplate.h"

using namespace rpg3D;

TeamTemplate::TeamTemplate()
	: super("team", ITeamModule::TYPE)
{
	//void
}

IEntityModule* TeamTemplate::createNewModuleIfExists(IWorldEntity* entity, BasicModuleTemplate* _t, std::shared_ptr<ExtraData> extraData, IEntityModuleCreator* moduleCreator) {
	TeamTemplate* t = dynamic_cast<TeamTemplate*>(_t);
	if(!t) {
		throw LogicException(LOC);
	}

	TeamModule* mod = new TeamModule(entity, t);

	return mod;
}

TeamTemplate::~TeamTemplate() {
	//void
}
