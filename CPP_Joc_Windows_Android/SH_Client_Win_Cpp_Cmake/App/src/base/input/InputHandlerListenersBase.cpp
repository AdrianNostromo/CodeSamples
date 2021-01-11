#include "InputHandlerListenersBase.h"

InputHandlerListenersBase::InputHandlerListenersBase(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler)
    : super(app, deviceInputConnection, errorHandler)
{
    //void
}

bool InputHandlerListenersBase::distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry) {
    return false;
}

InputHandlerListenersBase::~InputHandlerListenersBase() {
    //void
}
