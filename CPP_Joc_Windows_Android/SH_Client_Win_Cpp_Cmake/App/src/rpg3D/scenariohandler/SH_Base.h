#pragma once

#include <base/gh.h>
#include <worldGame3D/scenariohandler/ScenariosHandler.h>

namespace rpg3D {
class SH_Base : public worldGame3D::ScenariosHandler {priv typedef worldGame3D::ScenariosHandler super;pub dCtor(SH_Base);
    pub explicit SH_Base(
        IRenderingHandler* renderingHandler,
        IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
        IContainer2D* layer2DGameWorld,
        IApp* app);

    pub ~SH_Base() override;
};
};
