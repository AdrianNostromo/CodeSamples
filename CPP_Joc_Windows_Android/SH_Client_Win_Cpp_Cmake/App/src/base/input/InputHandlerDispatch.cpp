#include "InputHandlerDispatch.h"
#include <base/exceptions/LogicException.h>
#include <base/input/util/Touch.h>
#include <base/input/sensor/data/SensorDataInputHandler.h>
#include <base/input/sensor/event/SensorEvent.h>
#include <base/MM.h>

InputHandlerDispatch::InputHandlerDispatch(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler)
    : super(app, deviceInputConnection, errorHandler)
{
    //void
}

void InputHandlerDispatch::onLoopEvent_appLoop_inputDispatch(IEventListener& eventListener, base::LoopEvent& event) {
    super::onLoopEvent_appLoop_inputDispatch(eventListener, event);

    if(inputsCacheStack.count() > 0) {
        for(int i=0;i<inputsCacheStack.count();i++) {
            std::shared_ptr<base::SensorEvent>& inputCacheEntry = inputsCacheStack.getReference(i);

            // Dispatch the event to all listeners.
            if(!distributeInputCacheEntry(inputCacheEntry)) {
                // It is not mandatory to have a listener for each input (Eg. Device orientation is enabled by accelerometer but is not used.).
                //throw LogicException(LOC);
            }
        }

        inputsCacheStack.clear();

        std::shared_ptr<base::SensorEvent> event = std::make_shared<base::SensorEvent>(
            base::Touch::SensorType::InputHandler, base::SensorDataInputHandler::Type::EventsCacheBufferDispatchFinished,
            new base::SensorDataInputHandler()
        );
        distributeInputCacheEntry(event);
    }
}

InputHandlerDispatch::~InputHandlerDispatch() {
    //void
}
