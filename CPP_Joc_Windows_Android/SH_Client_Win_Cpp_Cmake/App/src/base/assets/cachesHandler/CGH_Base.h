#pragma once

#include <base/gh.h>
#include "ICachesGroupsHandler.h"
#include <base/object/ManagedObject.h>

namespace base {
class CGH_Base : public ManagedObject, virtual public ICachesGroupsHandler {
priv typedef ManagedObject super;

    pub dCtor(CGH_Base);
    pub explicit CGH_Base();

    pub ~CGH_Base() override;
};
};
