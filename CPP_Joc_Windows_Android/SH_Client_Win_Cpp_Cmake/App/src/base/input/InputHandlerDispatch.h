#pragma once

#include <base/gh.h>
#include "InputHandlerIndividualKeyboardKeyListeners.h"

class InputHandlerDispatch : public InputHandlerIndividualKeyboardKeyListeners {priv typedef InputHandlerIndividualKeyboardKeyListeners super;pub dCtor(InputHandlerDispatch);
    pub explicit InputHandlerDispatch(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler);

    prot void onLoopEvent_appLoop_inputDispatch(IEventListener& eventListener, base::LoopEvent& event) override;

    pub ~InputHandlerDispatch() override;
};
