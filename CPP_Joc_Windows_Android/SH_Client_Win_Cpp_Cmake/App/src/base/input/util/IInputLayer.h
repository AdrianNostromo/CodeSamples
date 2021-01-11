#pragma once

#include <base/gh.h>
#include <memory>
#include <functional>
#include <base/list/ArrayList.h>
#include <base/input/util/Touch.h>

class IEventListener;
namespace base {
    class SensorEvent;
}

namespace base {
class IInputLayer {
public:
    virtual void addSensorListener(
        base::Touch::SensorType* sensorType, int eventType,
        std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
        ArrayList<std::shared_ptr<IEventListener>>& autoListenersList
    ) = 0;
    virtual void addKeyboardKeySensorListener(
        int keyCode, int actionType,
        std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
        ArrayList<std::shared_ptr<IEventListener>>& autoListenersList
    ) = 0;

    virtual base::Touch::CursorMode* getCursorMode() = 0;
    virtual bool getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) = 0;
    virtual void setCursorMode(base::Touch::CursorMode* cursorMode) = 0;

    virtual ~IInputLayer() = default;

};
};
