#pragma once

#include <base/gh.h>
#include <worldGame3D/gw/interaction/deviceinput/ManagedDeviceInput.h>
#include "IManagedDeviceInput.h"

namespace rpg3D {
class MDI_Base : public base:: ManagedDeviceInput, virtual public IManagedDeviceInput {priv typedef base::ManagedDeviceInput super; pub dCtor(MDI_Base);
    priv base::IBlockingInputLayer* blockingInputLayer = nullptr;

    pub explicit MDI_Base(base::IGameWorld* gw);
    prot void createMain() override;

    pub base::IBlockingInputLayer* getBlockingInputLayer() override;

    pub void clearInput() override;

    prot virtual void disableUnitControlType(int old_unitControlType);
    prot virtual void enableUnitControlType(int unitControlType);

    prot void disposePre() override;
    pub ~MDI_Base() override;
};
};
