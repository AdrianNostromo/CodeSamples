#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include "IRuntimeSettings.h"

namespace base {
class RS_Base : public ManagedObject, public virtual IRuntimeSettings {priv typedef ManagedObject super;pub dCtor(RS_Base);
    pub explicit RS_Base();

    pub ~RS_Base() override;
};
};
