#include "ScenarioFactory.h"
#include "gw/GameWorld.h"
#include <base/statics/StaticsInit.h>
#include "gw/czTownA/config/CZ_TownA.h"
#include "gw/czGridBattleA/config/CZ_GridBattleA.h"

using namespace zoneAlert_a_v1;

base::IGameWorld* ScenarioFactory::GameWorldFactory::newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) {
	GameWorld* gameWorld = new GameWorld(handler, scenarioTemplate, scenarioConfig);

    return gameWorld;
}

ScenarioTemplate* ScenarioFactory::scenarioTemplate = base::StaticsInit::AddCbGeneral<ScenarioTemplate*>(1, []() {
	scenarioTemplate = ScenarioTemplate::newEntry(
		"zoneAlert_a_v1",
		new GameWorldFactory()
	);

	// Manually include the zones here to make sure their code get included somehere and linked.
	scenarioTemplate->addZoneFactory(czTownA::CZ_TownA::NAME, czTownA::CZ_TownA::FactoryFunction, true/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(czGridBattleA::CZ_GridBattleA::NAME, czGridBattleA::CZ_GridBattleA::FactoryFunction, false/*isDefaultScenario*/);
});
