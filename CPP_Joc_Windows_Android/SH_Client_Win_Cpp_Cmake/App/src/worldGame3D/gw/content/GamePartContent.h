#pragma once

#include "GPC_PlayerEntitySelector.h"

namespace base {
class GamePartContent : public GPC_PlayerEntitySelector {
private: typedef GPC_PlayerEntitySelector super;
public:
    GamePartContent(GamePartContent const&) = delete;
    GamePartContent(GamePartContent &&) = default;
    GamePartContent& operator=(GamePartContent const&) = delete;
    GamePartContent& operator=(GamePartContent &&) = default;

    explicit GamePartContent(IGameWorld* gw);

    ~GamePartContent() override;
};
};
