#include "InputHandlerCursor.h"

InputHandlerCursor::InputHandlerCursor(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler)
    : super(app, deviceInputConnection, errorHandler)
{
    //void
}

base::Touch::CursorMode* InputHandlerCursor::getCursorMode() {
    return deviceInputConnection->getCursorMode();
}

bool InputHandlerCursor::getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) {
    return deviceInputConnection->getIsCursorModeSupported(cursorMode);
}

void InputHandlerCursor::setCursorMode(base::Touch::CursorMode* cursorMode) {
    deviceInputConnection->setCursorMode(cursorMode);
}

InputHandlerCursor::~InputHandlerCursor() {
    //void
}
