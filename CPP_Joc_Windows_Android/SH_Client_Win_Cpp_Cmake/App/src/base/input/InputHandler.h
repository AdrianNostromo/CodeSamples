#pragma once

#include <base/gh.h>
#include "InputHBlockingStreamLayerListeners.h"

class InputHandler : public InputHBlockingStreamLayerListeners {priv typedef InputHBlockingStreamLayerListeners super;pub dCtor(InputHandler);
    pub explicit InputHandler(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler);

    pub ~InputHandler() override;
};
