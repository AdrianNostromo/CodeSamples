#pragma once

#include <worldGame3D/gw/gamepart/GamePart.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGPOnline.h"

namespace base {
class GPOBase : public GamePart, virtual public IGPOnline {
private: typedef GamePart super;
public:
    GPOBase(GPOBase const&) = delete;
    GPOBase(GPOBase &&) = default;
    GPOBase& operator=(GPOBase const&) = delete;
    GPOBase& operator=(GPOBase &&) = default;

    explicit GPOBase(IGameWorld* gw);

    ~GPOBase() override;
};
};
