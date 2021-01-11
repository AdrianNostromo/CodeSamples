#pragma once

#include "GPIInput.h"

namespace base {
class GPInteraction : public GPIInput {
private: typedef GPIInput super;
public:
    GPInteraction(GPInteraction const&) = delete;
    GPInteraction(GPInteraction &&) = default;
    GPInteraction& operator=(GPInteraction const&) = delete;
    GPInteraction& operator=(GPInteraction &&) = default;

    explicit GPInteraction(base::IGameWorld* gw);

    ~GPInteraction() override;
};
};
