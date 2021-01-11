#pragma once

#include <base/gh.h>
#include "SH_Base.h"

namespace rpg3D {
    class CustomisedContentHandler;
    class ICustomisedContentHandler;//#include <rpg3D/scenariohandler/customisedContentHandler/ICustomisedContentHandler.h>
};

namespace rpg3D {
class SH_CustomContent : public SH_Base {priv typedef SH_Base super;pub dCtor(SH_CustomContent);
    priv rpg3D::CustomisedContentHandler* customisedContentHandler = nullptr;

    pub explicit SH_CustomContent(
        IRenderingHandler* renderingHandler,
        IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
        IContainer2D* layer2DGameWorld,
        IApp* app);
    pub void create() override;

    prot rpg3D::ICustomisedContentHandler* getCustomisedContentHandler();

    prot void onActiveScenarioDeactivatePre(base::IGameWorld* gameWorld) override;
    prot void onActiveScenarioActivated(base::IGameWorld* gameWorld) override;

    pub ~SH_CustomContent() override;
};
};
