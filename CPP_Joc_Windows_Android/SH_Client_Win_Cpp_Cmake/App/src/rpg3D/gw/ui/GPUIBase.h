#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/gamepart/GamePart.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGamePartUserInterface.h"

namespace rpg3D {
class GPUIBase : public GamePart, virtual public IGamePartUserInterface {
private: typedef GamePart super;
public:
    GPUIBase(GPUIBase const&) = delete;
    GPUIBase(GPUIBase &&) = default;
    GPUIBase& operator=(GPUIBase const&) = delete;
    GPUIBase& operator=(GPUIBase &&) = default;

    explicit GPUIBase(base::IGameWorld* gw);

    ~GPUIBase() override;
};
};
