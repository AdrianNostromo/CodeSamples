#include "NI_Sensors.h"
#include <base/exceptions/LogicException.h>

using namespace base;

NI_Sensors::NI_Sensors()
	: super()
{
	//void
}

bool NI_Sensors::getIsSensorActive(base::Touch::SensorType* sensorId) {
    throw LogicException(LOC);
}

bool NI_Sensors::getIsSensorSupported(base::Touch::SensorType* sensorId) {
	return false;
}

void NI_Sensors::setIsSensorActiveIncremental(base::Touch::SensorType* sensorId, bool isActive) {
    throw LogicException(LOC);
}

NI_Sensors::~NI_Sensors() {
	//void
}
