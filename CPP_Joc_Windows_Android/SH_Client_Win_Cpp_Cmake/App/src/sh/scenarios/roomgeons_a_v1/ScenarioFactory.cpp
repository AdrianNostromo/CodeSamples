#include "ScenarioFactory.h"
#include "gw/GameWorld.h"
#include <base/statics/StaticsInit.h>
#include "gw/zoneArenaRoom/config/ContentZoneArenaRoom.h"
#include "gw/zoneTown/config/ContentZoneTown.h"

using namespace roomgeons_a_v1;

base::IGameWorld* ScenarioFactory::GameWorldFactory::newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) {
	GameWorld* gameWorld = new GameWorld(handler, scenarioTemplate, scenarioConfig);

    return gameWorld;
}

ScenarioTemplate* ScenarioFactory::scenarioTemplate = base::StaticsInit::AddCbGeneral<ScenarioTemplate*>(1, []() {
	scenarioTemplate = ScenarioTemplate::newEntry(
		"roomgeons_a_v1",
		new GameWorldFactory()
	);

	// Manually include the zones here to make sure their code get included somehere and linked.
	scenarioTemplate->addZoneFactory(Town::ContentZoneTown::NAME, Town::ContentZoneTown::FactoryFunction, true/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(ArenaRoom::ContentZoneArenaRoom::NAME, ArenaRoom::ContentZoneArenaRoom::FactoryFunction, false/*isDefaultScenario*/);
});
