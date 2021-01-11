#pragma once

#include "GPMBase.h"

namespace battlezones_a_v1 {
class GamePartMain : public GPMBase {
private: typedef GPMBase super;
public:
    GamePartMain(GamePartMain const&) = delete;
    GamePartMain(GamePartMain &&) = default;
    GamePartMain& operator=(GamePartMain const&) = delete;
    GamePartMain& operator=(GamePartMain &&) = default;

    explicit GamePartMain(base::IGameWorld* gw);

    ~GamePartMain() override;
};
};
