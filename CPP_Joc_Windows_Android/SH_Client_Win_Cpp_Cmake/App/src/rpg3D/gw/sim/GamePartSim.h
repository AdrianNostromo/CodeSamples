#pragma once

#include "GPSBase.h"

namespace rpg3D {
class GamePartSim : public GPSBase {
private: typedef GPSBase super;
public:
    GamePartSim(GamePartSim const&) = delete;
    GamePartSim(GamePartSim &&) = default;
    GamePartSim& operator=(GamePartSim const&) = delete;
    GamePartSim& operator=(GamePartSim &&) = default;

    explicit GamePartSim(base::IGameWorld* gw);

    ~GamePartSim() override;
};
};
