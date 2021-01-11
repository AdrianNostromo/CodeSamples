#pragma once

#include <base/gh.h>
#include "IRenderablesManager.h"
#include <base/object/ManagedObject.h>

namespace graphics {
    class IHandlerRenderablesManager;
};

namespace graphics {
class RM_Base : public base::ManagedObject, public virtual IRenderablesManager {priv typedef base::ManagedObject super;pub dCtor(RM_Base);
    pub IHandlerRenderablesManager* handler;

    pub explicit RM_Base();

    pub ~RM_Base() override;
};
};
