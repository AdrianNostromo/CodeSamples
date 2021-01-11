#pragma once

#include "GPCLActiveStarship.h"

namespace startarea_a_v1 {
class GPContentLocal : public GPCLActiveStarship {
private: typedef GPCLActiveStarship super;
public:
    GPContentLocal(GPContentLocal const&) = delete;
    GPContentLocal(GPContentLocal &&) = default;
    GPContentLocal& operator=(GPContentLocal const&) = delete;
    GPContentLocal& operator=(GPContentLocal &&) = default;

    explicit GPContentLocal(base::IGameWorld* gw);

    ~GPContentLocal() override;
};
};
