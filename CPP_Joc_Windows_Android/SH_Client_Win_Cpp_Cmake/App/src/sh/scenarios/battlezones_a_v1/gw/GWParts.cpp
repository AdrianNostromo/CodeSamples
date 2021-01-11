#include <sh/scenarios/battlezones_a_v1/gw/main/GamePartMain.h>
#include "GWParts.h"
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace battlezones_a_v1;

GWParts::GWParts(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
    : super(handler, scenarioTemplate, scenarioConfig)
{
    //void
}

base::GamePartMain* GWParts::newGPMain() {
	GamePartMain* ret = newt GamePartMain(this);

	return ret;
}

base::GamePartContent* GWParts::newGPContent() {
	GamePartContent* ret = newt GamePartContent(this);

	return ret;
}

base::GPOnline* GWParts::newGPOnline() {
	GPOnline* ret = newt GPOnline(this);

	online = ret;

	return ret;
}

base::GPManager* GWParts::newGPManager() {
	GPManager* ret = newt GPManager(this);

	return ret;
}

base::GamePartSimExtendable* GWParts::newGPSim() {
	GamePartSim* ret = newt GamePartSim(this);

	return ret;
}

GWParts::~GWParts() {
    //void
}
