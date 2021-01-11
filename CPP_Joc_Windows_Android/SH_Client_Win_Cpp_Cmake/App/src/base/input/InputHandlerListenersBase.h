#pragma once

#include <base/gh.h>
#include "InputHandlerCache.h"

class InputHandlerListenersBase : public InputHandlerCache {priv typedef InputHandlerCache super;pub dCtor(InputHandlerListenersBase);
    pub explicit InputHandlerListenersBase(IApp* app, INativeInput* deviceInputConnection, IErrorHandler* errorHandler);

    prot virtual bool distributeInputCacheEntry(std::shared_ptr<base::SensorEvent>& inputCacheEntry);

    pub ~InputHandlerListenersBase() override;
};
