#pragma once

#include <worldGame3D/gw/gamepart/GamePart.h>
#include <worldGame3D/gw/IGameWorld.h>
#include <worldGame3D/gw/interaction/GPInteraction.h>
#include "IGPInteraction.h"

namespace rpg3D {
class GPInteractionBase : public base::GPInteraction, virtual public IGPInteraction {
private: typedef base::GPInteraction super;
public:
    GPInteractionBase(GPInteractionBase const&) = delete;
    GPInteractionBase(GPInteractionBase &&) = default;
    GPInteractionBase& operator=(GPInteractionBase const&) = delete;
    GPInteractionBase& operator=(GPInteractionBase &&) = default;

    explicit GPInteractionBase(base::IGameWorld* gw);

    ~GPInteractionBase() override;
};
};
