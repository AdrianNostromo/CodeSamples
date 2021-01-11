#include "FlagEntry.h"

using namespace base;

base::FlagsEvent* FlagEntry::FlagsEvent_flagChanged = newt base::FlagsEvent("FlagsEvent_flagChanged");

FlagEntry::FlagEntry(std::string& name)
	: name(name)
{
	// void
}

std::shared_ptr<base::IWrappedValue> FlagEntry::getValue() {
	return value;
}

void FlagEntry::setValue(std::shared_ptr<base::IWrappedValue> value) {
	this->value = value;

	if (eventsDispatcher.getListenersCount() > 0) {
		eventsDispatcher.dispatch(*FlagsEvent_flagChanged->setData(this));
	}
}

base::EventDispatcher<base::FlagsEvent>& FlagEntry::getEventsDispatcher() {
	return eventsDispatcher;
}

FlagEntry::~FlagEntry() {
	//void
}
