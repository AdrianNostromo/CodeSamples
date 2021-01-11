#include "AppEvent.h"

using namespace base;

int AppEvent::typeCounter = 0;

AppEvent::AppEvent(std::string name)
	: name(std::move(name))
{
	type = ++typeCounter;
}

void AppEvent::clearData() {
	//void
}

AppEvent::~AppEvent() {
	//void
}
