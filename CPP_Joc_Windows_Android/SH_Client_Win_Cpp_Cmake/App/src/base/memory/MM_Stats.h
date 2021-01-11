#pragma once

#include <base/gh.h>
#include "MM_Base.h"
#include "util/MemoryStats.h"

namespace mem {
class MM_Stats : public MM_Base {priv typedef MM_Base super;pub dCtor(MM_Stats);
    prot MemoryStats stats{};

    pub explicit MM_Stats();

    pub MemoryStats* getStats() final;

    pub ~MM_Stats() override;
};
};
