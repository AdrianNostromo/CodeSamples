#pragma once

#include <base/gh.h>
#include <base/nativeInput/INativeInput.h>
#include "InputHandlerBase.h"

class IEventListener;
namespace base {
    class LoopEvent;
};

class InputHandlerCache : public InputHandlerBase, virtual public IInputProcessor {priv typedef InputHandlerBase super;pub dCtor(InputHandlerCache);
    pub INativeInput* deviceInputConnection;

    prot ArrayList<std::shared_ptr<base::SensorEvent>> inputsCacheStack{};

    priv float checkDelay = 0.0f;

    priv ArrayList<std::shared_ptr<IEventListener>> localAutoListenersList{};

    pub explicit InputHandlerCache(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler);
    prot void create() override;

    prot virtual void onLoopEvent_appLoop_inputDispatch(IEventListener& eventListener, base::LoopEvent& event);

    pub virtual void onSensorEvent(std::shared_ptr<base::SensorEvent>& sensorEvent) override;

    pub ~InputHandlerCache() override;

};
