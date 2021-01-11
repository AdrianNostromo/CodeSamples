#pragma once

#include <base/gh.h>
#include <base/event/dispatcher/TypedEventDispatcher1D.h>

namespace worldGame3D {
    namespace gw {
        class CapabilityEvent;
    };
};

namespace worldGame3D {
namespace gw {
class ICapabilities {
    // NOTE. DONT MODIFY. The capabilities are saved in sceario data and the values should never be edited.
    // This is used by tools to show that no capabilities are required.
    pub static unsigned char NOT_USED_INDEX;

    pub static unsigned char STACKABLES_HAULER_BIT_INDEX;
    // Note. When adding extra capabilities, add them to Capabilities::enableAllCapabilities().

    pub static CapabilityEvent* EV_CapabilityChanged;

    pub virtual base::TypedEventDispatcher1D<CapabilityEvent>& getEvents() = 0;

    pub virtual bool getWithBitIndex(unsigned char capBitIndex) = 0;
    pub virtual void setWithBitIndex(unsigned char capBitIndex, bool state) = 0;
    pub virtual bool hasWithBitIndex(unsigned char capBitIndex) = 0;

    // This is a convenience function, will be removed when optional capabilities are used more.
    pub virtual void enableAllCapabilities() = 0;

    pub virtual ~ICapabilities() = default;
};
};
};
