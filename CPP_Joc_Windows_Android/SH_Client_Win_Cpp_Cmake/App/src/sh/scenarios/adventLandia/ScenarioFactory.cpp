#include "ScenarioFactory.h"
#include "gw/GameWorld.h"
#include <base/statics/StaticsInit.h>
#include "gw/czTownA/config/CZ_TownA.h"
#include "gw/czDefenseTownA/config/CZ_DefenseTownA.h"

using namespace adventLandia;

base::IGameWorld* ScenarioFactory::GameWorldFactory::newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) {
	GameWorld* gameWorld = new GameWorld(handler, scenarioTemplate, scenarioConfig);

    return gameWorld;
}

ScenarioTemplate* ScenarioFactory::scenarioTemplate = base::StaticsInit::AddCbGeneral<ScenarioTemplate*>(1, []() {
	scenarioTemplate = ScenarioTemplate::newEntry(
		"adventLandia",
		new GameWorldFactory()
	);

	// Manually include the zones here to make sure their code get included somehere and linked.
	scenarioTemplate->addZoneFactory(czTownA::CZ_TownA::NAME, czTownA::CZ_TownA::FactoryFunction, true/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(czDefenseTownA::CZ_DefenseTownA::NAME, czDefenseTownA::CZ_DefenseTownA::FactoryFunction, false/*isDefaultScenario*/);
});
