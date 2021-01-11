#pragma once

#include <worldGame3D/gw/content/GamePartContent.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGPContentLocal.h"

namespace startarea_a_v1 {
class GPCLBase : public base::GamePartContent, virtual public IGPContentLocal {
private: typedef GamePartContent super;
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
