#pragma once

#include <base/gh.h>
#include <base/map/Map2D.h>
#include "InputHandlerSensorListeners.h"
#include <memory>
#include <base/event/dispatcher/TypedEventDispatcher2D.h>

class InputHandlerIndividualKeyboardKeyListeners : public InputHandlerSensorListeners {priv typedef InputHandlerSensorListeners super;pub dCtor(InputHandlerIndividualKeyboardKeyListeners);
    priv base::TypedEventDispatcher2D<std::shared_ptr<base::SensorEvent>> eventDispatcherIndividualKeyboardKey{};

    pub explicit InputHandlerIndividualKeyboardKeyListeners(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler);

    pub void addKeyboardKeySensorListener(
        int keyCode, int actionType,
        std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
        ArrayList<std::shared_ptr<IEventListener>>& autoListenersList
    ) override;

    prot bool distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) override;

    pub ~InputHandlerIndividualKeyboardKeyListeners() override;
};
