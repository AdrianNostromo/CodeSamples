#pragma once

#include <rpg3D/scenariohandler/ScenariosHandler.h>

namespace sh {
	class IAppSquareHeads;
};

namespace sh {
class SH_Base : public rpg3D::ScenariosHandler {
private: typedef rpg3D::ScenariosHandler super;
protected:
    IAppSquareHeads* appLocal;

public:
    SH_Base(SH_Base const&) = delete;
    SH_Base(SH_Base &&) = default;
    SH_Base& operator=(SH_Base const&) = delete;
    SH_Base& operator=(SH_Base &&) = default;

    explicit SH_Base(
        IRenderingHandler* renderingHandler,
        IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
        IContainer2D* layer2DGameWorld,
        IAppSquareHeads* appLocal);

    ~SH_Base() override;
};
};
