#pragma once

#include "GPManBase.h"

namespace base {
class GPManager : public GPManBase {
private: typedef GPManBase super;
public:
    GPManager(GPManager const&) = delete;
    GPManager(GPManager &&) = default;
    GPManager& operator=(GPManager const&) = delete;
    GPManager& operator=(GPManager &&) = default;

    explicit GPManager(IGameWorld* gw);

    ~GPManager() override;
};
};
