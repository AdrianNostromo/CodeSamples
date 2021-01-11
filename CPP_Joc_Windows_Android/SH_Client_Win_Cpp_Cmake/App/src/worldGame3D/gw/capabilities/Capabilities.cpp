#include "Capabilities.h"
#include <base/exceptions/LogicException.h>
#include <base/math/util/BoolUtil.h>
#include <worldGame3D/gw/capabilities/event/CapabilityEvent.h>

using namespace worldGame3D;
using namespace worldGame3D::gw;

int Capabilities::MAX_CAPS_COUNT = 32;

Capabilities::Capabilities() {
	//void
}

base::TypedEventDispatcher1D<CapabilityEvent>& Capabilities::getEvents() {
	return events;
}

bool Capabilities::getWithBitIndex(unsigned char capBitIndex) {
	if (capBitIndex >= MAX_CAPS_COUNT) {
		throw LogicException(LOC);
	}

	return BoolUtil::isBitAtIndex(capsBitMap, capBitIndex);
}

void Capabilities::setWithBitIndex(unsigned char capBitIndex, bool state) {
	if (capBitIndex >= MAX_CAPS_COUNT) {
		throw LogicException(LOC);
	}

	bool cState = BoolUtil::isBitAtIndex(capsBitMap, capBitIndex);
	if (cState == state) {
		return;
	}

	if (!state) {
		BoolUtil::clearBitAtIndexReference(capsBitMap, capBitIndex);
	} else {
		BoolUtil::setBitAtIndexReference(capsBitMap, capBitIndex);
	}

	events.dispatchEvent(*EV_CapabilityChanged->setData(capBitIndex, state));
}

bool Capabilities::hasWithBitIndex(unsigned char capBitIndex) {
	if (capBitIndex >= MAX_CAPS_COUNT) {
		throw LogicException(LOC);
	}

	return BoolUtil::isBitAtIndex(capsBitMap, capBitIndex);
}

void Capabilities::enableAllCapabilities() {
	setWithBitIndex(STACKABLES_HAULER_BIT_INDEX, true/*state*/);
}

Capabilities::~Capabilities() {
	//void
}
