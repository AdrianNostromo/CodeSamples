#pragma once

#include <worldGame3D/gw/interaction/deviceinput/ManagedDeviceInput.h>
#include <rpg3D/gw/interaction/deviceinput/ManagedDeviceInput.h>
#include "GPInteractionBase.h"

namespace rpg3D {
class GPIInput : public GPInteractionBase {
private: typedef GPInteractionBase super;
public:
    GPIInput(GPIInput const&) = delete;
    GPIInput(GPIInput &&) = default;
    GPIInput& operator=(GPIInput const&) = delete;
    GPIInput& operator=(GPIInput &&) = default;

    explicit GPIInput(base::IGameWorld* gw);

	ManagedDeviceInput* newInput() override;

    ~GPIInput() override;
};
};
