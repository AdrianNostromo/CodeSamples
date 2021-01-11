#include "AsyncFHEvent.h"

using namespace base;

int AsyncFHEvent::typeCounter = 0;

AsyncFHEvent::AsyncFHEvent(std::string name)
	: name(std::move(name))
{
	type = ++typeCounter;
}

void AsyncFHEvent::clearData() {
	data_string = nullptr;
	data_binary = nullptr;
}

AsyncFHEvent::~AsyncFHEvent() {
	//void
}
