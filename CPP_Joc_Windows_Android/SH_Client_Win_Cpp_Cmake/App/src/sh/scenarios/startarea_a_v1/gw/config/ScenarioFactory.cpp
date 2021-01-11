#include "ScenarioFactory.h"
#include <sh/scenarios/startarea_a_v1/gw/GameWorld.h>
#include <sh/scenarios/startarea_a_v1/gw/config/entity/EntityTemplatesList.h>
#include <base/statics/StaticsInit.h>
#include "zoneConfigs/ContentZoneMain.h"

using namespace startarea_a_v1;

base::IGameWorld* ScenarioFactory::ILocalWorldFactory::newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) {
	GameWorld* gameWorld = new GameWorld(handler, scenarioTemplate, scenarioConfig);

    return gameWorld;
}

ScenarioTemplate* ScenarioFactory::scenarioTemplate = base::StaticsInit::AddCbGeneral<ScenarioTemplate*>(1/*initLayerIndex*/, []() {
	scenarioTemplate = ScenarioTemplate::newEntry(
		"startarea_a_v1",
		new ILocalWorldFactory()
	);

	// Manually include the zones here to make sure their code get included somehere and linked.
	scenarioTemplate->addZoneFactory(Main::ContentZoneMain::NAME, Main::ContentZoneMain::FactoryFunction, true/*isDefaultScenario*/);
});
