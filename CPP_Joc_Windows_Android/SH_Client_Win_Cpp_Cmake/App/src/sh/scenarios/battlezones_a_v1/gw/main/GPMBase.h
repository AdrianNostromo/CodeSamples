#pragma once

#include <worldGame3D/gw/main/GamePartMain.h>

namespace battlezones_a_v1 {
class GPMBase : public base::GamePartMain {
private: typedef base::GamePartMain super;
public:
    GPMBase(GPMBase const&) = delete;
    GPMBase(GPMBase &&) = default;
    GPMBase& operator=(GPMBase const&) = delete;
    GPMBase& operator=(GPMBase &&) = default;

    explicit GPMBase(base::IGameWorld* gw);

    ~GPMBase() override;
};
};
