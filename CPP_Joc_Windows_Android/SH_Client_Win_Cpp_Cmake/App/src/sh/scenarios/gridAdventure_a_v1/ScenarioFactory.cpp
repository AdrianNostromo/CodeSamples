#include "ScenarioFactory.h"
#include "gw/GameWorld.h"
#include <base/statics/StaticsInit.h>
#include "gw/zoneMain/config/ContentZoneMain.h"
#include "gw/zoneGridBattleA/config/ContentZoneGridBattleA.h"

using namespace gridAdventure_a_v1;

base::IGameWorld* ScenarioFactory::GameWorldFactory::newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) {
	GameWorld* gameWorld = new GameWorld(handler, scenarioTemplate, scenarioConfig);

    return gameWorld;
}

ScenarioTemplate* ScenarioFactory::scenarioTemplate = base::StaticsInit::AddCbGeneral<ScenarioTemplate*>(1, []() {
	scenarioTemplate = ScenarioTemplate::newEntry(
		"gridAdventure_a_v1",
		new GameWorldFactory()
	);

	// Manually include the zones here to make sure their code get included somehere and linked.
	scenarioTemplate->addZoneFactory(Main::ContentZoneMain::NAME, Main::ContentZoneMain::FactoryFunction, true/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(GridBattleA::ContentZoneGridBattleA::NAME, GridBattleA::ContentZoneGridBattleA::FactoryFunction, false/*isDefaultScenario*/);
});
