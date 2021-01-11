#pragma once

#include <rpg3D/gw/manager/GPManager.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGPManager.h"

namespace battlezones_a_v1 {
class GPManBase : public rpg3D::GPManager, virtual public IGPManager {
private: typedef rpg3D::GPManager super;
protected:
    base::IGameWorld* gw;
    
public:
    GPManBase(GPManBase const&) = delete;
    GPManBase(GPManBase &&) = default;
    GPManBase& operator=(GPManBase const&) = delete;
    GPManBase& operator=(GPManBase &&) = default;

    explicit GPManBase(base::IGameWorld* gw);

    virtual void gwInitFinished();

    ~GPManBase() override;
};
};
