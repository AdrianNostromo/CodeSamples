#include "ScenarioFactory.h"
#include "../GWLocal.h"
#include <base/statics/StaticsInit.h>

using namespace battlezones_a_v1;

base::IGameWorld* ScenarioFactory::GameWorldFactory::newInstance(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig) {
	GWLocal* gameWorld = new GWLocal(handler, scenarioTemplate, scenarioConfig);

	return gameWorld;
}

ScenarioTemplate* ScenarioFactory::scenarioTemplate = base::StaticsInit::AddCbGeneral<ScenarioTemplate*>(1, []() {
	scenarioTemplate = ScenarioTemplate::newEntry(
		"battlezones_a_v1",
		new GameWorldFactory()
	);

	// Manually include the zones here to make sure their code get included somehere and linked.
	//void
});
