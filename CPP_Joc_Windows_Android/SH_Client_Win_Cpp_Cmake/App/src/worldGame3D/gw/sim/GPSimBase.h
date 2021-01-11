#pragma once

#include <worldGame3D/gw/gamepart/GamePart.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGPSim.h"

namespace base {
class GPSimBase : public GamePart, virtual public IGPSim {
private: typedef GamePart super;
public:
    GPSimBase(GPSimBase const&) = delete;
    GPSimBase(GPSimBase &&) = default;
    GPSimBase& operator=(GPSimBase const&) = delete;
    GPSimBase& operator=(GPSimBase &&) = default;

    explicit GPSimBase(IGameWorld* gw);

    virtual void createComponents();

    virtual void isPlayEnabled_changedEvent();

    ~GPSimBase() override;
};
};
