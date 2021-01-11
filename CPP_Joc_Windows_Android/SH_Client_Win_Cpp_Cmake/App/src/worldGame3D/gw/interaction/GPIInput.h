#pragma once

#include <worldGame3D/gw/interaction/deviceinput/ManagedDeviceInput.h>
#include "GPInteractionBase.h"

namespace base {
class GPIInput : public GPInteractionBase {
private: typedef GPInteractionBase super;
protected:
	ManagedDeviceInput* input = nullptr;

public:
    GPIInput(GPIInput const&) = delete;
    GPIInput(GPIInput &&) = default;
    GPIInput& operator=(GPIInput const&) = delete;
    GPIInput& operator=(GPIInput &&) = default;

    explicit GPIInput(base::IGameWorld* gw);

	virtual ManagedDeviceInput* newInput();
	IManagedDeviceInput* getInput() override;

    ~GPIInput() override;
protected:
    void createMain() override;
    void createBMain() override;

    void disposePre() override;
    void disposeMain() override;

};
};
