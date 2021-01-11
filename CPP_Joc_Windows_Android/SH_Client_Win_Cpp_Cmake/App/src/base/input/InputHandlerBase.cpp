#include "InputHandlerBase.h"

InputHandlerBase::InputHandlerBase(IApp* app, IErrorHandler* errorHandler)
    : super(),
    app(app),
    errorHandler(errorHandler)
{
    //void
}

InputHandlerBase::~InputHandlerBase() {
    //void
}
