#pragma once

#include <base/gh.h>
#include "SH_ActiveScenario.h"

namespace worldGame3D {
class ScenariosHandler : public SH_ActiveScenario {priv typedef SH_ActiveScenario super;pub dCtor(ScenariosHandler);
	pub explicit ScenariosHandler(
        IRenderingHandler* renderingHandler,
        IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
        IContainer2D* layer2DGameWorld,
		IApp* app);
	
	pub ~ScenariosHandler() override;
};
};
