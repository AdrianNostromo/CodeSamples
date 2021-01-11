#pragma once

#include "SH_Base.h"

namespace sh {
class SH_SquareHeads : public SH_Base {
private: typedef SH_Base super;
public:
    SH_SquareHeads(SH_SquareHeads const&) = delete;
    SH_SquareHeads(SH_SquareHeads &&) = default;
    SH_SquareHeads& operator=(SH_SquareHeads const&) = delete;
    SH_SquareHeads& operator=(SH_SquareHeads &&) = default;

    explicit SH_SquareHeads(
        IRenderingHandler* renderingHandler,
        IAppAssets* appAssets, IAppMetrics* appMetrics, IAppUtils* appUtils,
        IContainer2D* layer2DGameWorld,
        IAppSquareHeads* appLocal);

    ~SH_SquareHeads() override;
protected:
    void onGWEvent(std::string id, std::shared_ptr<base::GWEventDataBase> data) override;

};
};
