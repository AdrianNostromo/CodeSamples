#include "InputHandlerIndividualKeyboardKeyListeners.h"
#include <base/input/sensor/event/SensorEvent.h>
#include <base/input/util/Touch.h>
#include <base/input/sensor/data/SensorDataKeyboard.h>

InputHandlerIndividualKeyboardKeyListeners::InputHandlerIndividualKeyboardKeyListeners(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler)
    : super(app, deviceInputConnection, errorHandler)
{
    //void
}

void InputHandlerIndividualKeyboardKeyListeners::addKeyboardKeySensorListener(
    int keyCode, int actionType,
    std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    base::EventDispatcher<std::shared_ptr<base::SensorEvent>>* d = eventDispatcherIndividualKeyboardKey.getDispatcher(keyCode, actionType, true);
    d->addListener(cb, &autoListenersList);
}

bool InputHandlerIndividualKeyboardKeyListeners::distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) {
    bool ret = super::distributeInputCacheEntry(inputCacheEntry);

    if(inputCacheEntry->sensorType == base::Touch::SensorType::Keyboard && inputCacheEntry->eventType == base::SensorDataKeyboard::Type::Key) {
        base::SensorDataKeyboard* sensorData = inputCacheEntry->data->castToSubtype<base::SensorDataKeyboard*>();

        int ct = 0;
        base::EventDispatcher<std::shared_ptr<base::SensorEvent>>* d = eventDispatcherIndividualKeyboardKey.getDispatcher(sensorData->keyCode, sensorData->actionType, false);
        if(d != nullptr) {
            ct += d->dispatch(inputCacheEntry);
        }
        if(ct > 0) {
            ret = true;
        }
    }

    return ret;
}

InputHandlerIndividualKeyboardKeyListeners::~InputHandlerIndividualKeyboardKeyListeners() {
    //void
}
