#include "InputHandlerCache.h"
#include <base/exceptions/LogicException.h>
#include <base/appLoop/event/LoopEvent.h>
#include <base/appLoop/ILoopDistribution.h>
#include <base/app/IApp.h>

InputHandlerCache::InputHandlerCache(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler)
    : super(app, errorHandler), deviceInputConnection(deviceInputConnection)
{
    //void
}

void InputHandlerCache::create() {
    super::create();
    
    app->getLoopDistribution()->addEventListener(
        base::ILoopDistribution::AppLoopEvent_inputDispatch->type,
        std::bind(&InputHandlerCache::onLoopEvent_appLoop_inputDispatch, this, std::placeholders::_1, std::placeholders::_2),
        localAutoListenersList
    );

    deviceInputConnection->setInputProcessor(this);
}

void InputHandlerCache::onLoopEvent_appLoop_inputDispatch(IEventListener& eventListener, base::LoopEvent& event) {
    // Do some error detection.
    checkDelay -= event.deltaS;
    if (checkDelay <= 0) {
        checkDelay = 3.0f;

        if (deviceInputConnection->getInputProcessor() != this) {
            errorHandler->handleAppCriticalError("Error", "InputHandlerProcessor.appLoop.1.");

            throw LogicException(LOC);
        }
    }
}

void InputHandlerCache::onSensorEvent(std::shared_ptr<base::SensorEvent>& sensorEvent)
{
    inputsCacheStack.appendReference(sensorEvent);
}

InputHandlerCache::~InputHandlerCache() {
    //void
}
