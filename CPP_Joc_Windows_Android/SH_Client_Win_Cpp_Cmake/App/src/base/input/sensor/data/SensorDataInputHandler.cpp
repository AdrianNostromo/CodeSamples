#include "SensorDataInputHandler.h"
#include <base/input/util/Touch.h>

using namespace base;

const int SensorDataInputHandler::Type::EventsCacheBufferDispatchFinished = 1;

SensorDataInputHandler::SensorDataInputHandler() {
	rawSubclassPointer = this;
}

SensorDataInputHandler::~SensorDataInputHandler() {
	//void
}
