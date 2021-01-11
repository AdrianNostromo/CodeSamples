#pragma once

#include <worldGame3D/gw/gamepart/GamePart.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGPInteraction.h"

namespace base {
class GPInteractionBase : public GamePart, virtual public IGPInteraction {
private: typedef GamePart super;
public:
    GPInteractionBase(GPInteractionBase const&) = delete;
    GPInteractionBase(GPInteractionBase &&) = default;
    GPInteractionBase& operator=(GPInteractionBase const&) = delete;
    GPInteractionBase& operator=(GPInteractionBase &&) = default;

    explicit GPInteractionBase(base::IGameWorld* gw);

    ~GPInteractionBase() override;
};
};