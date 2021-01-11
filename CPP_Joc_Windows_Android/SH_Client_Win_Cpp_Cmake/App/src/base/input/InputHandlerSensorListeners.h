#pragma once

#include <base/gh.h>
#include "InputHandlerListenersBase.h"
#include <base/list/ArrayList.h>
#include <base/event/dispatcher/EventDispatcher.h>
#include <base/event/dispatcher/TypedEventDispatcher2D.h>

class InputHandlerSensorListeners : public InputHandlerListenersBase {priv typedef InputHandlerListenersBase super;pub dCtor(InputHandlerSensorListeners);
	// Use shared_ptr to avoid large map block memory usage on change.
	priv base::TypedEventDispatcher2D<std::shared_ptr<base::SensorEvent>> eventDispatcher{};

    pub explicit InputHandlerSensorListeners(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler);

	pub void addSensorListener(
		base::Touch::SensorType* sensorType, int eventType,
		std::function<void(IEventListener& eventListener, std::shared_ptr<base::SensorEvent>& event)> cb,
		ArrayList<std::shared_ptr<IEventListener>>& autoListenersList
	) override;
	priv int dispatchSensorEvent(std::shared_ptr<base::SensorEvent>& event);

    prot bool distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) override;

	pub ~InputHandlerSensorListeners() override;
};
