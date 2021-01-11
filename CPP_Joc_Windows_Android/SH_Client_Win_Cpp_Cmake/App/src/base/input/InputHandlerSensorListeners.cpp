#include "InputHandlerSensorListeners.h"
#include <base/exceptions/LogicException.h>
#include <base/input/sensor/event/SensorEvent.h>
#include <base/input/util/Touch.h>
#include <base/input/sensor/data/SensorDataInputHandler.h>

InputHandlerSensorListeners::InputHandlerSensorListeners(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler)
    : super(app, deviceInputConnection, errorHandler)
{
    //void
}

void InputHandlerSensorListeners::addSensorListener(
	base::Touch::SensorType* sensorType, int eventType,
	std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
	ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
	base::EventDispatcher<std::shared_ptr<base::SensorEvent>>* d = eventDispatcher.getDispatcher(sensorType->index, eventType, true);
	d->addListener(cb, &autoListenersList);
}

int InputHandlerSensorListeners::dispatchSensorEvent(std::shared_ptr<base::SensorEvent>& event) {
	int ct = 0;

	base::EventDispatcher<std::shared_ptr<base::SensorEvent>>* d = eventDispatcher.getDispatcher(event->sensorType->index, event->eventType, false);
	if(d != nullptr) {
		ct += d->dispatch(event);
	}

	return ct;
}

bool InputHandlerSensorListeners::distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) {
	bool ret = super::distributeInputCacheEntry(inputCacheEntry);

	int ct = dispatchSensorEvent(inputCacheEntry);
	if(ct > 0) {
		ret = true;
	}

    return ret;
}

InputHandlerSensorListeners::~InputHandlerSensorListeners() {
    //void
}
