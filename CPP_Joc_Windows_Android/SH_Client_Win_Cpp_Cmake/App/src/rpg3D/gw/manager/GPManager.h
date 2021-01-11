#pragma once

#include "GPManPlatforms.h"

namespace rpg3D {
class GPManager : public GPManPlatforms {
private: typedef GPManPlatforms super;
public:
    GPManager(GPManager const&) = delete;
    GPManager(GPManager &&) = default;
    GPManager& operator=(GPManager const&) = delete;
    GPManager& operator=(GPManager &&) = default;

    explicit GPManager(base::IGameWorld* gw);

    ~GPManager() override;

};
};
