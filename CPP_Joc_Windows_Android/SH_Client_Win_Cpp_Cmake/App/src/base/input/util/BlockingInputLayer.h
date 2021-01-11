#pragma once

#include <base/gh.h>
#include <base/input/util/IBlockingInputLayer.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <functional>
#include <base/event/dispatcher/TypedEventDispatcher2D.h>
#include "Touch.h"

namespace base {
class BlockingInputLayer : virtual public IBlockingInputLayer {
    priv IHandler* handler = nullptr;

    priv base::TypedEventDispatcher2D<std::shared_ptr<base::SensorEvent>> eventDispatcher{};
    priv base::TypedEventDispatcher2D<std::shared_ptr<base::SensorEvent>> eventDispatcherIndividualKeyboardKey{};

    base::Touch::CursorMode* cursorMode = base::Touch::CursorMode::Normal;

    // [sensorIndex][state_int_bool]
    // 0: false; 1: true;
    Map1D<int, int> sensorStatesMap{};

    pub explicit BlockingInputLayer();

    pub IHandler* getHandler() override;
    pub void setHandler(IHandler* handler) override;

    pub void addSensorListener(
        base::Touch::SensorType* sensorType, int eventType,
        std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
        ArrayList<std::shared_ptr<IEventListener>>& autoListenersList
    ) override;
    priv int dispatchSensorEvent(std::shared_ptr<base::SensorEvent>& event);

    pub void addKeyboardKeySensorListener(
        int keyCode, int actionType,
        std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
        ArrayList<std::shared_ptr<IEventListener>>& autoListenersList
    ) override;

    pub base::Touch::CursorMode* getCursorMode() override;
    pub bool getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) override;
    pub void setCursorMode(base::Touch::CursorMode* cursorMode) override;

    pub bool getIsSensorActive(base::Touch::SensorType* sensorId) override;
    pub bool getIsSensorSupported(base::Touch::SensorType* sensorId) override;
    pub void setIsSensorActive(base::Touch::SensorType* sensorId, bool isActive) override;

    pub bool distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) override;

    pub ~BlockingInputLayer() override;
};
};
