#include "FlagsEvent.h"
#include <worldGame3D/gw/entity/IWorldEntity.h>

using namespace base;

int FlagsEvent::typeCounter = 0;

FlagsEvent::FlagsEvent(std::string name)
	: name(std::move(name))
{
	type = ++typeCounter;
}

void FlagsEvent::clearData() {
	flag = nullptr;
}

FlagsEvent* FlagsEvent::setData(base::FlagEntry* flag) {
	this->flag = flag;

	return this;
}

FlagsEvent::~FlagsEvent() {
	//void
}
