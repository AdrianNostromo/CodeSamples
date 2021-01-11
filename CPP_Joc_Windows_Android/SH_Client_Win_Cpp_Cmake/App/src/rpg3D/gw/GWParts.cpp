#include "GWParts.h"
#include <rpg3D/gw/ui/GamePartUserInterface.h>
#include <rpg3D/gw/content/GamePartContent.h>
#include <rpg3D/gw/sim/GamePartSim.h>
#include <rpg3D/gw/contentZones/GamePartContentZones.h>
#include <rpg3D/gw/manager/GPManager.h>
#include <rpg3D/gw/physics3D/GamePartPhysics3D.h>
#include <rpg3D/gw/customisedContent/GamePartCustomisedContent.h>
#include <rpg3D/gw/userEntitiesController/GPUserEntitiesController.h>
#include <base/MM.h>
#include <base/memory/IMemoryManager.h>

using namespace rpg3D;

GWParts::GWParts(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig)
	: super(handler, scenarioTemplate, scenarioConfig)
{
    //void
}

void GWParts::createGParts() {
	super::createGParts();

	addGamePartUninitialised(newt rpg3D::GamePartContentZones(this));
	
	// These 2 variables are for the subclass gp.
	addGamePartUninitialised(newGPUserInterface());

	addGamePartUninitialised(newt rpg3D::GamePartPhysics3D(this));

	addGamePartUninitialised(newt rpg3D::GamePartCustomisedContent(this));

	addGamePartUninitialised(newt rpg3D::GPUserEntitiesController(this));
}

GamePartUserInterface* GWParts::newGPUserInterface() {
	GamePartUserInterface* ret = newt GamePartUserInterface(this);

	return ret;
}

base::GamePartContent* GWParts::newGPContent() {
	GamePartContent* ret = newt GamePartContent(this);

	return ret;
}

base::GamePartSimExtendable* GWParts::newGPSim() {
	GamePartSim* ret = newt GamePartSim(this);

	return ret;
}

base::GPInteraction* GWParts::newGPInteraction() {
	GPInteraction* ret = newt GPInteraction(this);

	return ret;
}

base::GPManager* GWParts::newGPManager() {
	GPManager* ret = newt GPManager(this);

	return ret;
}

GWParts::~GWParts() {
    //void
}
