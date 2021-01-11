#include "SensorEvent.h"
#include <base/input/sensor/data/SensorData.h>

using namespace base;

SensorEvent::SensorEvent(base::Touch::SensorType* sensorType, int eventType, SensorData* data)
    : sensorType(sensorType), eventType(eventType), data(data)
{
	//void
}

SensorEvent::~SensorEvent() {
    if(data != nullptr) {
        delete data;
        data = nullptr;
    }
}
