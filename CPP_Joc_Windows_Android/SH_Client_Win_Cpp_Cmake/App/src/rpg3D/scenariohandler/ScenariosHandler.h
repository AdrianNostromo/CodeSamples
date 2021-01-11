#pragma once

#include <base/gh.h>
#include "SH_CustomContent.h"

namespace rpg3D {
class ScenariosHandler : public SH_CustomContent {priv typedef SH_CustomContent super;pub dCtor(ScenariosHandler);
    pub explicit ScenariosHandler(
        IRenderingHandler* renderingHandler,
        IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
        IContainer2D* layer2DGameWorld,
        IApp* app);

    pub ~ScenariosHandler() override;
};
};
