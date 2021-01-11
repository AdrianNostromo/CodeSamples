#pragma once

#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include "IGPContent.h"

namespace base {
class GPC_Base : public GamePart, virtual public IGPContent {
private: typedef GamePart super;
public:
    GPC_Base(GPC_Base const&) = delete;
    GPC_Base(GPC_Base &&) = default;
    GPC_Base& operator=(GPC_Base const&) = delete;
    GPC_Base& operator=(GPC_Base &&) = default;

    explicit GPC_Base(IGameWorld* gw);

    ~GPC_Base() override;
};
};
