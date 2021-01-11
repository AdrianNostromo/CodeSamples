#include "TeamModule.h"

using namespace rpg3D;

TeamModule::TeamModule(
    IWorldEntity* entity,
    TeamTemplate* t)
    : super(t->moduleName, entity),
      t(t)
{
    //void
}

int TeamModule::getTeamIndex() {
	return teamIndex;
}

void TeamModule::setTeamIndex(int teamIndex) {
	this->teamIndex = teamIndex;
}

TeamModule::~TeamModule() {
	//void
}
