#include "ScenarioFactory.h"
#include "../GameWorld.h"
#include <base/statics/StaticsInit.h>
#include "../../zones/main/config/ContentZoneMain.h"
#include "../../zones/mazeTD/config/ContentZoneMazeTD.h"
#include "../../zones/pathTD/config/ContentZonePathTD.h"
#include "../../zones/storyAOverworld/config/ContentZoneStoryAOverworld.h"

using namespace towerDefense_a_v1;

base::IGameWorld* ScenarioFactory::GameWorldFactory::newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) {
	GameWorld* gameWorld = new GameWorld(handler, scenarioTemplate, scenarioConfig);

    return gameWorld;
}

ScenarioTemplate* ScenarioFactory::scenarioTemplate = base::StaticsInit::AddCbGeneral<ScenarioTemplate*>(1, []() {
	scenarioTemplate = ScenarioTemplate::newEntry(
		"towerDefense_a_v1",
		new GameWorldFactory()
	);

	// Manually include the zones here to make sure their code get included somehere and linked.
	scenarioTemplate->addZoneFactory(Main::ContentZoneMain::NAME, Main::ContentZoneMain::FactoryFunction, true/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(MazeTD::ContentZoneMazeTD::NAME, MazeTD::ContentZoneMazeTD::FactoryFunction, false/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(PathTD::ContentZonePathTD::NAME, PathTD::ContentZonePathTD::FactoryFunction, false/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(StoryAOverworld::ContentZoneStoryAOverworld::NAME, StoryAOverworld::ContentZoneStoryAOverworld::FactoryFunction, false/*isDefaultScenario*/);
});
