#include "ScenarioFactory.h"
#include "../GameWorld.h"
#include <base/statics/StaticsInit.h>
#include "zoneConfigs/ContentZoneMain.h"

using namespace spaceShipMissions_a_v1;

base::IGameWorld* ScenarioFactory::GameWorldFactory::newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) {
	GameWorld* gameWorld = new GameWorld(handler, scenarioTemplate, scenarioConfig);

    return gameWorld;
}

ScenarioTemplate* ScenarioFactory::scenarioTemplate = base::StaticsInit::AddCbGeneral<ScenarioTemplate*>(1, []() {
	scenarioTemplate = ScenarioTemplate::newEntry(
		"spaceShipMissions_a_v1",
		new GameWorldFactory()
	);

	// Manually include the zones here to make sure their code get included somehere and linked.
	scenarioTemplate->addZoneFactory(Main::ContentZoneMain::NAME, Main::ContentZoneMain::FactoryFunction, true/*isDefaultScenario*/);
});
