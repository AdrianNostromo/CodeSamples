#pragma once

#include <rpg3D/gw/content/GamePartContent.h>
#include "IGPContent.h"
#include <worldGame3D/gw/IGameWorld.h>

namespace battlezones_a_v1 {
class GPCLBase : public rpg3D::GamePartContent, virtual public IGPContent {
private: typedef rpg3D::GamePartContent super;
protected:
    base::IGameWorld* gw;

public:
    GPCLBase(GPCLBase const&) = delete;
    GPCLBase(GPCLBase &&) = default;
    GPCLBase& operator=(GPCLBase const&) = delete;
    GPCLBase& operator=(GPCLBase &&) = default;

    explicit GPCLBase(base::IGameWorld* gw);

    ~GPCLBase() override;
};
};
