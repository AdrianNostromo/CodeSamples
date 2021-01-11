#include "SensorDataRotationsPitchRollOriented.h"
#include <base/input/util/Touch.h>

using namespace base;

const int SensorDataRotationsPitchRollOriented::Type::Data = 1;

SensorDataRotationsPitchRollOriented::SensorDataRotationsPitchRollOriented(float pitchDeg, float pitchDeg_delta, float rollDeg, float rollDeg_delta)
	: pitchDeg(pitchDeg), pitchDeg_delta(pitchDeg_delta),
	  rollDeg(rollDeg), rollDeg_delta(rollDeg_delta)
{
	rawSubclassPointer = this;
}

SensorDataRotationsPitchRollOriented::~SensorDataRotationsPitchRollOriented() {
	//void
}
