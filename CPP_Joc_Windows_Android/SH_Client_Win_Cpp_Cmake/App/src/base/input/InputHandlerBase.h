#pragma once

#include <base/gh.h>
#include <base/component/ComponentsHandler.h>
#include <base/error/IErrorHandler.h>
#include "IInputHandler.h"
#include <base/component/IComponent_AppComponent.h>

class IApp;

class InputHandlerBase : public base::ManagedObject, virtual public IInputHandler, public virtual IComponent_AppComponent {priv typedef base::ManagedObject super;pub dCtor(InputHandlerBase);
    prot IApp* app;

    prot IErrorHandler* errorHandler;

    pub explicit InputHandlerBase(IApp* app, IErrorHandler* errorHandler);

    pub ~InputHandlerBase() override;
};
