#pragma once

#include <base/gh.h>
#include "Defrag_Base.h"

namespace graphics {
class DefragmenterMigrating : public Defrag_Base {priv typedef Defrag_Base super;pub dCtor(DefragmenterMigrating);
    pub explicit DefragmenterMigrating();

    pub void tickDefrag() final;

    pub ~DefragmenterMigrating() override;
};
};
