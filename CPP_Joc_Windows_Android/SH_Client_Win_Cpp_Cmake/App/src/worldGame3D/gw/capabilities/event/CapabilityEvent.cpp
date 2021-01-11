#include "CapabilityEvent.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace worldGame3D;
using namespace worldGame3D::gw;

int CapabilityEvent::typeCounter = 0;

CapabilityEvent::CapabilityEvent(std::string name)
	: name(std::move(name))
{
	type = ++typeCounter;
}

void CapabilityEvent::clearData() {
	capabilityBitIndex = 0;
	state = false;
}

CapabilityEvent* CapabilityEvent::setData(unsigned char capabilityBitIndex, bool state) {
	this->capabilityBitIndex = capabilityBitIndex;
	this->state = state;

	return this;
}

CapabilityEvent::~CapabilityEvent() {
	//void
}
