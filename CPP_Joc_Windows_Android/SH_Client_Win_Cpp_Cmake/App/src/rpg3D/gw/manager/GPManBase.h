#pragma once

#include <worldGame3D/gw/manager/GPManager.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGPManager.h"

namespace rpg3D {
class GPManBase : public base::GPManager, virtual public IGPManager {
private: typedef base::GPManager super;
public:
    GPManBase(GPManBase const&) = delete;
    GPManBase(GPManBase &&) = default;
    GPManBase& operator=(GPManBase const&) = delete;
    GPManBase& operator=(GPManBase &&) = default;

    explicit GPManBase(base::IGameWorld* gw);

    ~GPManBase() override;
};
};
