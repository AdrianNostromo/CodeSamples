#pragma once

#include <worldGame3D/gw/content/GamePartContent.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGPContent.h"

namespace rpg3D {
class GPCBase : public base::GamePartContent, virtual public IGPContent {
private: typedef base::GamePartContent super;
protected:
	base::IGameWorld* gw;

public:
    GPCBase(GPCBase const&) = delete;
    GPCBase(GPCBase &&) = default;
    GPCBase& operator=(GPCBase const&) = delete;
    GPCBase& operator=(GPCBase &&) = default;

    explicit GPCBase(base::IGameWorld* gw);

    ~GPCBase() override;
};
};
