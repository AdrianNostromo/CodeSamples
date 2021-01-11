#include "ICapabilities.h"
#include <worldGame3D/gw/capabilities/event/CapabilityEvent.h>

using namespace worldGame3D;
using namespace worldGame3D::gw;

unsigned char ICapabilities::NOT_USED_INDEX = 255;

unsigned char ICapabilities::STACKABLES_HAULER_BIT_INDEX = 0;
// Note. When adding extra capabilities, add them to Capabilities::enableAllCapabilities().

CapabilityEvent* ICapabilities::EV_CapabilityChanged = new CapabilityEvent("EV_CapabilityChanged");
