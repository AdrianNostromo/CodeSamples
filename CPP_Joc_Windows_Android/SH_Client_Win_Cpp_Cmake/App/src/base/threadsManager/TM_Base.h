#pragma once

#include <base/gh.h>
#include <base/object/ManagedObject.h>
#include "IThreadsManager.h"

namespace base {
class TM_Base : public base::ManagedObject, virtual public IThreadsManager {priv typedef base::ManagedObject super;pub dCtor(TM_Base);
    pub explicit TM_Base();

    pub ~TM_Base() override;
};
};
