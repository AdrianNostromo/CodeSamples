#pragma once

#include <worldGame3D/gw/sim/GamePartSimExtendable.h>
#include "IGamePartSim.h"

namespace rpg3D {
class GPSBase : public base::GamePartSimExtendable, virtual public IGamePartSim {
private: typedef GamePartSimExtendable super;
public:
    GPSBase(GPSBase const&) = delete;
    GPSBase(GPSBase &&) = default;
    GPSBase& operator=(GPSBase const&) = delete;
    GPSBase& operator=(GPSBase &&) = default;

    explicit GPSBase(base::IGameWorld* gw);

    ~GPSBase() override;
};
};
