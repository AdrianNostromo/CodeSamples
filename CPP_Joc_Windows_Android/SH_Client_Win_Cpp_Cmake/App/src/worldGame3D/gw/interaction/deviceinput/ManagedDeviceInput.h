#pragma once

#include <base/math/Vector3.h>
#include <base/input/util/Keyboard.h>
#include "MDIBase.h"

namespace base {
class ManagedDeviceInput : public MDIBase {
private: typedef MDIBase super;
public:
    ManagedDeviceInput(ManagedDeviceInput const&) = delete;
    ManagedDeviceInput(ManagedDeviceInput &&) = default;
    ManagedDeviceInput& operator=(ManagedDeviceInput const&) = delete;
    ManagedDeviceInput& operator=(ManagedDeviceInput &&) = default;

    explicit ManagedDeviceInput(IGameWorld* gw);

    ~ManagedDeviceInput() override;
};
};
