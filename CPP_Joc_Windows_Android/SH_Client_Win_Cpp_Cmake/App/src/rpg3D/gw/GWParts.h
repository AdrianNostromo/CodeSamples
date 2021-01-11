#pragma once

#include <base/gh.h>
#include "GWBase.h"
#include <rpg3D/gw/interaction/GPInteraction.h>

namespace rpg3D {
	class GamePartUserInterface;
};

namespace rpg3D {
class GWParts : public GWBase {priv typedef GWBase super;pub dCtor(GWParts);
	pub explicit GWParts(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

	prot void createGParts() override;

	prot virtual GamePartUserInterface* newGPUserInterface();
	prot base::GamePartContent* newGPContent() override;
	prot base::GamePartSimExtendable* newGPSim() override;
	prot base::GPInteraction* newGPInteraction() override;
	prot base::GPManager* newGPManager() override;

    pub ~GWParts() override;

};
};
