#include "GameWorld.h"
#include "init/GamePartInit.h"
#include <worldGame3D/gw/capabilities/ICapabilities.h>

using namespace spaceShipMissions_a_v1;

GameWorld::GameWorld(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
	: super(handler, scenarioTemplate, scenarioConfig)
{
	//void
}

void GameWorld::createGParts() {
	super::createGParts();

	getCapabilities().enableAllCapabilities();

	addGamePartUninitialised(new GamePartInit(this));
}

void GameWorld::startLevelPlay() {
	super::startLevelPlay();

	setIsPlayEnabled(true);
}

GameWorld::~GameWorld() {
	//void
}
