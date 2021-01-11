#pragma once

#include "GPCBase.h"

namespace rpg3D {
class GamePartContent : public GPCBase {
private: typedef GPCBase super;
public:
    GamePartContent(GamePartContent const&) = delete;
    GamePartContent(GamePartContent &&) = default;
    GamePartContent& operator=(GamePartContent const&) = delete;
    GamePartContent& operator=(GamePartContent &&) = default;

    explicit GamePartContent(base::IGameWorld* gw);

    ~GamePartContent() override;
};
};
