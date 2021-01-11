#include "GameWorld.h"
#include "init/GamePartInit.h"
#include "online/GPOnline.h"
#include "main/GamePartMain.h"
#include "content/GPContentLocal.h"
#include "manager/GPManager.h"
#include "ui/GamePartUserInterface.h"
#include <worldGame3D/gw/capabilities/ICapabilities.h>

using namespace startarea_a_v1;

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

void GameWorld::createPost() {
	super::createPost();

	getManagerAs<GPManager*>()->gwInitFinished();
}

rpg3D::GamePartUserInterface* GameWorld::newGPUserInterface() {
	GamePartUserInterface* ret = new GamePartUserInterface(this);

	IGamePartUserInterface* iPointer = ret;

	return ret;
}

base::GamePartMain* GameWorld::newGPMain() {
	GamePartMain* ret = new GamePartMain(this);

	return ret;
}

base::GamePartContent* GameWorld::newGPContent() {
	GPContentLocal* ret = new GPContentLocal(this);

	return ret;
}

base::GPOnline* GameWorld::newGPOnline() {
	GPOnline* ret = new GPOnline(this);

	return ret;
}

base::GPManager* GameWorld::newGPManager() {
	GPManager* ret = new GPManager(this);

	return ret;
}

void GameWorld::startLevelPlay() {
	super::startLevelPlay();

	setIsPlayEnabled(true);
}

GameWorld::~GameWorld() {
	//void
}
