#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/manager/GPManager.h>
#include "GWPartOnline.h"

class GWPartManager : public GWPartOnline {priv typedef GWPartOnline super;pub dCtor(GWPartManager);
	priv base::GPManager* manager = nullptr;

	pub explicit GWPartManager(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

	pub base::IGPManager* getManager() override;

	prot void createGParts() override;

	prot virtual base::GPManager* newGPManager();

	prot void disposeMain() override;

	pub ~GWPartManager() override;
};
