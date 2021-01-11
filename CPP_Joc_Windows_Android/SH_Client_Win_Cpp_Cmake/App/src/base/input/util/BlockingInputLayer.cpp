#include "BlockingInputLayer.h"
#include <base/input/sensor/event/SensorEvent.h>
#include <memory>
#include <base/input/sensor/data/SensorDataKeyboard.h>

using namespace base;

BlockingInputLayer::BlockingInputLayer() {
    //void
}

BlockingInputLayer::IHandler* BlockingInputLayer::getHandler() {
    return handler;
}

void BlockingInputLayer::setHandler(IHandler* handler) {
    this->handler = handler;
}

void BlockingInputLayer::addSensorListener(
    base::Touch::SensorType* sensorType, int eventType,
    std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    base::EventDispatcher<std::shared_ptr<base::SensorEvent>>* d = eventDispatcher.getDispatcher(sensorType->index, eventType, true);
    d->addListener(cb, &autoListenersList);
}

int BlockingInputLayer::dispatchSensorEvent(std::shared_ptr<base::SensorEvent>& event) {
    int ct = 0;

    base::EventDispatcher<std::shared_ptr<base::SensorEvent>>* d = eventDispatcher.getDispatcher(event->sensorType->index, event->eventType, false);
    if(d != nullptr) {
        ct += d->dispatch(event);
    }

    return ct;
}

void BlockingInputLayer::addKeyboardKeySensorListener(
    int keyCode, int actionType,
    std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
    ArrayList<std::shared_ptr<IEventListener>>& autoListenersList)
{
    base::EventDispatcher<std::shared_ptr<base::SensorEvent>>* d = eventDispatcherIndividualKeyboardKey.getDispatcher(keyCode, actionType, true);
    d->addListener(cb, &autoListenersList);
}

base::Touch::CursorMode* BlockingInputLayer::getCursorMode() {
    return cursorMode;
}

bool BlockingInputLayer::getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) {
    return handler->getIsCursorModeSupported(cursorMode);
}

void BlockingInputLayer::setCursorMode(base::Touch::CursorMode* cursorMode) {
    if(this->cursorMode == cursorMode) {
        return ;
    }

    this->cursorMode = cursorMode;

    if(handler != nullptr) {
        handler->onCursorModeChanged(this);
    }
}

bool BlockingInputLayer::getIsSensorActive(base::Touch::SensorType* sensorId) {
    bool isActive = sensorStatesMap.getDirectB((int)sensorId->index, 0) == 1 ? true : false;

    return isActive;
}

bool BlockingInputLayer::getIsSensorSupported(base::Touch::SensorType* sensorId) {
    return handler->getIsSensorSupported(sensorId);
}

void BlockingInputLayer::setIsSensorActive(base::Touch::SensorType* sensorId, bool isActive) {
    if(getIsSensorActive(sensorId) == isActive) {
        return ;
    }

    sensorStatesMap.putDirectB((int)sensorId->index, isActive ? 1 : 0);

    if(handler != nullptr) {
        handler->onIsSensorActiveChanged(sensorId, isActive);
    }
}

bool BlockingInputLayer::distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) {
    bool ret = dispatchSensorEvent(inputCacheEntry);

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

BlockingInputLayer::~BlockingInputLayer() {
    //void
}
