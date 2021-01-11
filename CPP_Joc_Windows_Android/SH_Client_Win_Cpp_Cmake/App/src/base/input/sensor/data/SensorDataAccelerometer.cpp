#include "SensorDataAccelerometer.h"
#include <base/input/util/Touch.h>

using namespace base;

const int SensorDataAccelerometer::Type::Data = 1;

SensorDataAccelerometer::SensorDataAccelerometer(float forceX, float forceY, float forceZ)
	: forceX(forceX), forceY(forceY), forceZ(forceZ)
{
	rawSubclassPointer = this;
}

SensorDataAccelerometer::~SensorDataAccelerometer() {
	//void
}
