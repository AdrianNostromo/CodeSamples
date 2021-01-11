#include "SensorDataToolExtraActivation.h"
#include <base/input/util/Touch.h>

using namespace rpg3D;

const int SensorDataToolExtraActivation::Type::Data_ListControl_Down_MainBtn = 1;
const int SensorDataToolExtraActivation::Type::Data_SecondaryControl_Down_MainBtn = 2;

SensorDataToolExtraActivation::SensorDataToolExtraActivation(int toolInstanceId, int sourceId)
	: toolInstanceId(toolInstanceId), sourceId(sourceId)
{
	rawSubclassPointer = this;
}

SensorDataToolExtraActivation::~SensorDataToolExtraActivation() {
	//void
}
