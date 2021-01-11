#include "LoopEvent.h"

using namespace base;

int LoopEvent::typeCounter = 0;

LoopEvent::LoopEvent(std::string name)
	: name(std::move(name))
{
	type = ++typeCounter;
}

void LoopEvent::clearData() {
	//void
}

LoopEvent* LoopEvent::setLoopData(float deltaS, int deltaMS, float appTimeS) {
	this->deltaS = deltaS;
	this->deltaMS = deltaMS;
	this->appTimeS = appTimeS;

	return this;
}

LoopEvent::~LoopEvent() {
	//void
}
