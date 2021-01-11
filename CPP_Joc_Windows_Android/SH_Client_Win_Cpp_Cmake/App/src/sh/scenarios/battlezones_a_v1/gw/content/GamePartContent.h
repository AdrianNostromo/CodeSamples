#pragma once

#include "GPCBase.h"

namespace battlezones_a_v1 {
class GamePartContent : public GPCLBase {
private: typedef GPCLBase super;
public:
    GamePartContent(GamePartContent const&) = delete;
    GamePartContent(GamePartContent &&) = default;
    GamePartContent& operator=(GamePartContent const&) = delete;
    GamePartContent& operator=(GamePartContent &&) = default;

    explicit GamePartContent(base::IGameWorld* gw);

    ~GamePartContent() override;
};
};
