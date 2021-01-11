#pragma once

#include <worldGame3D/gw/gamepart/GamePart.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGPManager.h"

namespace base {
class GPManBase : public GamePart, virtual public IGPManager {
private: typedef GamePart super;
public:
    GPManBase(GPManBase const&) = delete;
    GPManBase(GPManBase &&) = default;
    GPManBase& operator=(GPManBase const&) = delete;
    GPManBase& operator=(GPManBase &&) = default;

    explicit GPManBase(IGameWorld* gw);

    ~GPManBase() override;

};
};
