#include "SensorDataDeviceOrientation.h"
#include <base/input/util/Touch.h>

using namespace base;

const int SensorDataDeviceOrientation::Type::Data = 1;

SensorDataDeviceOrientation::SensorDataDeviceOrientation(base::Touch::DeviceOrientation* deviceOrientation)
	: deviceOrientation(deviceOrientation)
{
	rawSubclassPointer = this;
}

SensorDataDeviceOrientation::~SensorDataDeviceOrientation() {
	//void
}
