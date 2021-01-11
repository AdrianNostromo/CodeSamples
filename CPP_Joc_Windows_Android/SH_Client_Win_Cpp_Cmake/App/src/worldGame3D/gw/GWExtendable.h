#pragma once

#include <base/gh.h>
#include "GWPartManager.h"

namespace base {
class GWExtendable : public GWPartManager {priv typedef GWPartManager super;pub dCtor(GWExtendable);
	pub explicit GWExtendable(IHandler_GameWorld* handler, ScenarioTemplate* scenarioTemplate, ScenarioConfig* scenarioConfig);

	prot void createGParts() override;

	pub ~GWExtendable() override;
};
};
