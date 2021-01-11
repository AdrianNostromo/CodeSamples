#pragma once

#include <base/gh.h>
#include "IMemoryManager.h"

namespace mem {
class MM_Base : public virtual IMemoryManager {pub dCtor(MM_Base);
    pub explicit MM_Base();

    pub ~MM_Base() override;
};
};
