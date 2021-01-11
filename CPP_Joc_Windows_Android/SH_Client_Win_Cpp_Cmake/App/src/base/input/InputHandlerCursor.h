#pragma once

#include <base/gh.h>
#include "InputHandlerDispatch.h"

class InputHandlerCursor : public InputHandlerDispatch {priv typedef InputHandlerDispatch super;pub dCtor(InputHandlerCursor);
    pub explicit InputHandlerCursor(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler);

    pub base::Touch::CursorMode* getCursorMode() override;
    pub bool getIsCursorModeSupported(base::Touch::CursorMode* cursorMode) override;
    pub void setCursorMode(base::Touch::CursorMode* cursorMode) override;

    pub ~InputHandlerCursor() override;
};
