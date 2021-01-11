#pragma once

#include <rpg3D/gw/sim/GamePartSim.h>
#include "IGamePartSim.h"

namespace battlezones_a_v1 {
class GPSBase : public rpg3D::GamePartSim, virtual public IGamePartSim {
private: typedef rpg3D::GamePartSim super;
public:
    GPSBase(GPSBase const&) = delete;
    GPSBase(GPSBase &&) = default;
    GPSBase& operator=(GPSBase const&) = delete;
    GPSBase& operator=(GPSBase &&) = default;

    explicit GPSBase(base::IGameWorld* gw);

    ~GPSBase() override;
};
};
