#pragma once

#include <base/gh.h>
#include "GCEG_EntsGroups.h"

namespace worldGame3D {
class GCEntityGroups : public GCEG_EntsGroups {priv typedef GCEG_EntsGroups super;pub dCtor(GCEntityGroups);
    pub explicit GCEntityGroups(base::IGameWorld* gw);

    pub ~GCEntityGroups() override;
};
};
