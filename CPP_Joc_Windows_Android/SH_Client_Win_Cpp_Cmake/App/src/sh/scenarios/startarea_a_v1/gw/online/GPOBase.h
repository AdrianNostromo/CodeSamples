#pragma once

#include <worldGame3D/gw/online/GPOnline.h>
#include <worldGame3D/gw/IGameWorld.h>
#include "IGPOnline.h"

namespace startarea_a_v1 {
class GPOBase : public base::GPOnline, virtual public IGPOnline {
private: typedef base::GPOnline super;
protected:
    base::IGameWorld* gw;

public:
    GPOBase(GPOBase const&) = delete;
    GPOBase(GPOBase &&) = default;
    GPOBase& operator=(GPOBase const&) = delete;
    GPOBase& operator=(GPOBase &&) = default;

    explicit GPOBase(base::IGameWorld* gw);

    ~GPOBase() override;
};
};
