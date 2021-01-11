#pragma once

#include "SH_SquareHeads.h"

namespace sh {
class ScenariosHandler : public SH_SquareHeads {
private: typedef SH_SquareHeads super;
public:
    ScenariosHandler(ScenariosHandler const&) = delete;
    ScenariosHandler(ScenariosHandler &&) = default;
    ScenariosHandler& operator=(ScenariosHandler const&) = delete;
    ScenariosHandler& operator=(ScenariosHandler &&) = default;

    explicit ScenariosHandler(
        IRenderingHandler* renderingHandler,
        IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
        IContainer2D* layer2DGameWorld,
        IAppSquareHeads* appLocal);

    ~ScenariosHandler() override;
protected:
    void onGWEvent(std::string id, std::shared_ptr<base::GWEventDataBase> data) override;

};
};
