#pragma once

#include <base/gh.h>
#include "ICapabilities.h"

namespace worldGame3D {
namespace gw {
class Capabilities : public virtual ICapabilities {pub dCtor(Capabilities);
    // Modify the capsBitMap and increase this when needed.
    priv static int MAX_CAPS_COUNT;

    priv int capsBitMap = 0;

    prot base::TypedEventDispatcher1D<CapabilityEvent> events{};

    pub explicit Capabilities();

    pub base::TypedEventDispatcher1D<CapabilityEvent>& getEvents() final;

    pub bool getWithBitIndex(unsigned char capBitIndex) final;
    pub void setWithBitIndex(unsigned char capBitIndex, bool state) final;
    pub bool hasWithBitIndex(unsigned char capBitIndex) final;

    pub void enableAllCapabilities() final;

    pub ~Capabilities() override;
};
};
};
