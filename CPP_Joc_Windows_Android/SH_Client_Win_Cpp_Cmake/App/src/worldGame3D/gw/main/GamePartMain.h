#pragma once

#include "GPMTouchInputHandler.h"

namespace base {
class GamePartMain : public GPMTouchInputHandler {
private: typedef GPMTouchInputHandler super;
public:
    GamePartMain(GamePartMain const&) = delete;
    GamePartMain(GamePartMain &&) = default;
    GamePartMain& operator=(GamePartMain const&) = delete;
    GamePartMain& operator=(GamePartMain &&) = default;

    explicit GamePartMain(base::IGameWorld* gw);

    ~GamePartMain() override;
};
};
