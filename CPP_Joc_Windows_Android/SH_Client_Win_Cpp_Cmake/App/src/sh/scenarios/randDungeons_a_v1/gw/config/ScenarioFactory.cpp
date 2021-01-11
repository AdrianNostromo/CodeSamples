#include "ScenarioFactory.h"
#include "../GameWorld.h"
#include <base/statics/StaticsInit.h>
#include "../zoneMain/config/ContentZoneMain.h"
#include "../zoneRDungeonA/config/ContentZoneRDungeonA.h"
#include "../zoneMyRoomsListA/config/ContentZoneMyRoomsListA.h"
#include "../zonePrefabRoomsDungeonA/config/ContentZonePrefabRoomsDungeonA.h"
#include "../zoneTurnBasedRDungeonA/config/ContentZoneTurnBasedRDungeonA.h"

using namespace randDungeons_a_v1;

base::IGameWorld* ScenarioFactory::GameWorldFactory::newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) {
	GameWorld* gameWorld = new GameWorld(handler, scenarioTemplate, scenarioConfig);

    return gameWorld;
}

ScenarioTemplate* ScenarioFactory::scenarioTemplate = base::StaticsInit::AddCbGeneral<ScenarioTemplate*>(1, []() {
	scenarioTemplate = ScenarioTemplate::newEntry(
		"randDungeons_a_v1",
		new GameWorldFactory()
	);

	// Manually include the zones here to make sure their code get included somehere and linked.
	scenarioTemplate->addZoneFactory(Main::ContentZoneMain::NAME, Main::ContentZoneMain::FactoryFunction, true/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(RDungeonA::ContentZoneRDungeonA::NAME, RDungeonA::ContentZoneRDungeonA::FactoryFunction, false/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(MyRoomsListA::ContentZoneMyRoomsListA::NAME, MyRoomsListA::ContentZoneMyRoomsListA::FactoryFunction, false/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(PrefabRoomsDungeonA::ContentZonePrefabRoomsDungeonA::NAME, PrefabRoomsDungeonA::ContentZonePrefabRoomsDungeonA::FactoryFunction, false/*isDefaultScenario*/);
	scenarioTemplate->addZoneFactory(TurnBasedRDungeonA::ContentZoneTurnBasedRDungeonA::NAME, TurnBasedRDungeonA::ContentZoneTurnBasedRDungeonA::FactoryFunction, false/*isDefaultScenario*/);
});
